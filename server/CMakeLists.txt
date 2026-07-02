add_library(project_options INTERFACE)
target_compile_options(project_options INTERFACE
    -Wall
    -Wextra
    -Wpedantic
)

add_executable(tcp_socket_server
    src/main.c
    src/server.c
    src/protocol.c
    src/app_state.c
)

target_include_directories(tcp_socket_server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(tcp_socket_server PRIVATE project_options)

