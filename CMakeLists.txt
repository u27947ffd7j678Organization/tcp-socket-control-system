cmake_minimum_required(VERSION 3.22)

project(tcp_socket_control_system
    VERSION 0.1.0
    DESCRIPTION "TCP/IP socket control system learning project"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_subdirectory(server)

