cmake_minimum_required(VERSION 3.14)
project(client)

include_directories(
        ${PROJECT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/../mayhem
        ${PROJECT_SOURCE_DIR}/../mayhem/include
)

add_executable(
        ${PROJECT_NAME}
        main.cpp
        ../ext/ya_getopt-1.0.0/ya_getopt.c
)

target_link_libraries(
        ${PROJECT_NAME}
        fmt-header-only
        mayhem
        SDL2_ttf
        SDL2-static
)
