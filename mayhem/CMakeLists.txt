cmake_minimum_required(VERSION 3.14)
project(mayhem)

include_directories("${PROJECT_SOURCE_DIR}")

add_library(
        ${PROJECT_NAME}

        types.h types.cpp

        common/bytes.h
        common/defer.h
        common/rune.h common/rune.cpp
        common/result.h common/result_message.h
        common/memory_pool.h common/memory_pool.cpp
        common/string_support.h common/string_support.cpp
        common/term_stream_builder.h common/term_stream_builder.cpp
)
target_link_libraries(
        ${PROJECT_NAME}
        utf8proc
        fmt-header-only
)