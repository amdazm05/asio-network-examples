cmake_minimum_required(VERSION 3.0)
project(basr_tcp)

include_directories(
    ${PROJECT_SOURCE_DIR}/libs/asio/include
)

add_library(
    ${PROJECT_NAME}
    STATIC
    ${PROJECT_SOURCE_DIR}/src/tcp_server.cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include/
)

add_executable(
    server
    ./examples/barebone/server.cc
)

target_link_libraries(
    server
PUBLIC
    ${PROJECT_NAME}
    
)

add_executable(
    client
    ./examples/barebone/client.cc
)


add_executable(
    client_poll_example
    ./examples/barebone/client_poll_example.cc
)

target_link_libraries(
    client_poll_example
PUBLIC
    ${PROJECT_NAME}

)

add_executable(
    server_poll_and_read
    ./examples/barebone/server_poll_and_read.cc
)

target_link_libraries(
    server_poll_and_read
PUBLIC
    ${PROJECT_NAME}

)