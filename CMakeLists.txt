cmake_minimum_required(VERSION 3.22)

project(tcp_socket_control_system
    VERSION 0.1.0
    DESCRIPTION "TCP/IP socket control system learning project"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

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
