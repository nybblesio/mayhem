cmake_minimum_required(VERSION 3.14)
project(mayhem)

include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR})

add_library(
        ${PROJECT_NAME}

        include/mayhem/game.h

        log.h log.cpp
        game.h game.cpp
        input.h input.cpp
        types.h types.cpp
        sound.h sound.cpp
        video.h video.cpp
        timer.h timer.cpp
        window.h window.cpp
        boot_state.h boot_state.cpp
        editor_state.h editor_state.cpp
        bank_manager.h bank_manager.cpp
        state_machine.h state_machine.cpp

        common/bytes.h
        common/defer.h
        common/rune.h common/rune.cpp
        common/result.h common/result_message.h
        common/memory_pool.h common/memory_pool.cpp
        common/string_support.h common/string_support.cpp
        common/term_stream_builder.h common/term_stream_builder.cpp

        ../ext/SDL_FontCache/SDL_FontCache.c ../ext/SDL_FontCache/SDL_FontCache.h
)
target_link_libraries(
        ${PROJECT_NAME}
        utf8proc
        fmt-header-only
        SDL2_ttf
        SDL2-static
        fmod
)