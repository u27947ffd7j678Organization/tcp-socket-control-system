#include "server.h"

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "protocol.h"

#define RECV_BUFFER_SIZE 1024
#define LISTEN_BACKLOG 5

static int send_all(int client_fd, const char *message)
{
    size_t total_sent = 0;
    size_t message_length = strlen(message);

    while (total_sent < message_length) {
        ssize_t sent = send(
            client_fd,
            message + total_sent,
            message_length - total_sent,
            0
        );

        if (sent < 0) {
            perror("send");
            return -1;
        }

        if (sent == 0) {
            fprintf(stderr, "send: connection closed before all data was sent\n");
            return -1;
        }

        total_sent += (size_t)sent;
    }

    return 0;
}

static int create_listen_socket(int port)
{
    // socket() creates an IPv4 TCP endpoint.
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        return -1;
    }

    int reuse = 1;
    // SO_REUSEADDR lets the server restart quickly after it exits.
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt");
        close(server_fd);
        return -1;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons((uint16_t)port);

    // bind() assigns the local IP address and TCP port to the socket.
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_fd);
        return -1;
    }

    // listen() switches the socket into passive mode so clients can connect.
    if (listen(server_fd, LISTEN_BACKLOG) < 0) {
        perror("listen");
        close(server_fd);
        return -1;
    }

    return server_fd;
}

int server_run(AppState *state, int port)
{
    printf("[INFO] TCP server starting on port %d\n", port);

    int server_fd = create_listen_socket(port);
    if (server_fd < 0) {
        return -1;
    }

    printf("[INFO] Waiting for client connection...\n");

    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // accept() blocks until one client connects.
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0) {
        perror("accept");
        close(server_fd);
        return -1;
    }

    char client_ip[INET_ADDRSTRLEN] = "unknown";
    if (inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip)) != NULL) {
        printf("[INFO] Client connected: %s\n", client_ip);
    } else {
        printf("[INFO] Client connected\n");
    }

    bool should_close = false;
    char input_buffer[RECV_BUFFER_SIZE];
    size_t input_length = 0;

    while (!should_close) {
        char recv_buffer[RECV_BUFFER_SIZE];

        // recv() reads bytes sent by the connected client.
        ssize_t received = recv(client_fd, recv_buffer, sizeof(recv_buffer) - 1, 0);
        if (received < 0) {
            if (errno == EINTR) {
                continue;
            }

            perror("recv");
            break;
        }

        if (received == 0) {
            printf("[INFO] Client disconnected\n");
            break;
        }

        recv_buffer[received] = '\0';
        printf("[RECV] %s", recv_buffer);
        if (recv_buffer[received - 1] != '\n') {
            printf("\n");
        }

        if (input_length + (size_t)received >= sizeof(input_buffer)) {
            const char *error_response = "ERROR INVALID_FORMAT\n";
            printf("[SEND] %s", error_response);
            if (send_all(client_fd, error_response) < 0) {
                break;
            }
            input_length = 0;
            continue;
        }

        memcpy(input_buffer + input_length, recv_buffer, (size_t)received);
        input_length += (size_t)received;
        input_buffer[input_length] = '\0';

        char *line_start = input_buffer;
        char *line_end = strchr(line_start, '\n');

        while (line_end != NULL) {
            char command_line[RECV_BUFFER_SIZE];
            char response[PROTOCOL_RESPONSE_SIZE];
            size_t line_length = (size_t)(line_end - line_start + 1);

            if (line_length >= sizeof(command_line)) {
                snprintf(response, sizeof(response), "ERROR INVALID_FORMAT\n");
            } else {
                memcpy(command_line, line_start, line_length);
                command_line[line_length] = '\0';

                ProtocolResult result = protocol_handle_command(
                    state,
                    command_line,
                    response,
                    sizeof(response)
                );

                if (result == PROTOCOL_CLOSE) {
                    should_close = true;
                }
            }

            printf("[SEND] %s", response);

            // send() writes the generated protocol response back to the client.
            if (send_all(client_fd, response) < 0) {
                should_close = true;
                break;
            }

            if (should_close) {
                break;
            }

            line_start = line_end + 1;
            line_end = strchr(line_start, '\n');
        }

        if (line_start != input_buffer) {
            size_t remaining_length = input_length - (size_t)(line_start - input_buffer);
            memmove(input_buffer, line_start, remaining_length);
            input_length = remaining_length;
            input_buffer[input_length] = '\0';
        }

        if (input_length == sizeof(input_buffer) - 1) {
            const char *error_response = "ERROR INVALID_FORMAT\n";
            printf("[SEND] %s", error_response);
            if (send_all(client_fd, error_response) < 0) {
                break;
            }
            input_length = 0;
        }
    }

    if (input_length > 0 && !should_close) {
        char response[PROTOCOL_RESPONSE_SIZE];
        input_buffer[input_length] = '\0';

        ProtocolResult result = protocol_handle_command(
            state,
            input_buffer,
            response,
            sizeof(response)
        );

        printf("[SEND] %s", response);

        if (send_all(client_fd, response) < 0) {
            close(client_fd);
            close(server_fd);
            return 0;
        }

        if (result == PROTOCOL_CLOSE) {
            should_close = true;
        }
    }

    if (should_close) {
        printf("[INFO] Client disconnected\n");
    }

    close(client_fd);
    close(server_fd);
    printf("[INFO] TCP server stopped\n");

    return 0;
}
