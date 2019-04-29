// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#include <SDL.h>
#include "game.h"
#include "timer.h"
#include "boot_state.h"

namespace mayhem {

    static state_machine s_machine{};

    bool game_config_load(common::result& r, game_config_t& config) {
        return true;
    }

    bool game_config_save(common::result& r, game_config_t& config) {
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////

    bool game_run(common::result& r, game_t& game) {
        const color_t white = {.r = 0xff, .g = 0xff, .b = 0xff, .a = 0xff};

        uint16_t fps = 0;
        uint16_t frame_count = 0;
        uint32_t last_time = SDL_GetTicks();
        uint32_t last_fps_time = last_time;

        while (!SDL_QuitRequested()) {
            game.ticks = SDL_GetTicks();

            if (key_pressed(SDL_SCANCODE_ESCAPE)) {
                SDL_Event evt {};
                evt.type = SDL_EventType::SDL_QUIT;
                SDL_PushEvent(&evt);
            }

            if (!sound_update(r, game.sound))
                return false;

            if (!timer_update(r, game))
                return false;

            if (!s_machine.update(r, game))
                return false;

            if (game.config.show_fps) {
                if (!video_queue_text(
                        r,
                        game,
                        bank_id_t{0xff, 0},
                        white,
                        2,
                        2,
                        fmt::format("FPS:{:03}", fps))) {
                    return false;
                }
            }

            if (!video_update(r, game))
                return false;

            uint32_t frame_duration = SDL_GetTicks() - game.ticks;

            uint32_t fps_dt = last_time - last_fps_time;
            if (fps_dt >= 1000) {
                fps = frame_count;
                frame_count = 0;
                last_fps_time = last_time;
            }

            ++frame_count;

            if (frame_duration < milliseconds_per_frame) {
                SDL_Delay(milliseconds_per_frame - frame_duration);
            }

            last_time = SDL_GetTicks();
        }

        return true;
    }

    bool game_init(common::result& r, game_t& game) {
        auto result = SDL_Init(SDL_INIT_EVENTS|
                               SDL_INIT_GAMECONTROLLER|
                               SDL_INIT_JOYSTICK|
                               SDL_INIT_HAPTIC|
                               SDL_INIT_TIMER|
                               SDL_INIT_VIDEO);
        if (result < 0) {
            r.error("V001", "unable to initialize sdl.");
            return false;
        }

        log_init();

        if (!sound_init(r, game.sound))
            return false;

        if (!key_init(r))
            return false;

        if (!joystick_init(r))
            return false;

        if (!joystick_open(r, game.joystick))
            return false;

        if (!window_create(r, game.window, game.config.window_x, game.config.window_y))
            return false;

        game.video.bg_size.w = 32;
        game.video.bg_size.h = 32;

        game.video.tile_size.w = 32;
        game.video.tile_size.h = 32;

        game.video.max_sprites = 256;
        game.video.sprite_size.w = 16;
        game.video.sprite_size.h = 16;

        game.video.max_bg_size.w = 64;
        game.video.max_bg_size.h = 64;

        if (!video_init(r, game))
            return false;

        if (!s_machine.register_state<boot_state>(boot_state::type))
            return false;

        if (!s_machine.push(r, game, boot_state::type))
            return false;

        return true;
    }

    bool game_shutdown(common::result& r, game_t& game) {
        if (!sound_shutdown(r, game.sound)) {

        }

        if (!joystick_close(r, game.joystick)) {

        }

        if (!video_shutdown(r, game)) {

        }

        if (!window_destroy(r, game.window)) {

        }

        return true;
    }

}