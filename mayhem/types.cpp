// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#include <fmt/format.h>
#include <fmod_errors.h>
#include <SDL_keyboard.h>
#include "types.h"

namespace mayhem {

    bool window_create(
            common::result& r,
            window_t& window,
            int32_t x,
            int32_t y) {
        log_message(log_category_t::video, "create SDL window.");
        window.sx = scale_x;
        window.sy = scale_y;
        window.w = screen_width;
        window.h = screen_height;
        window.handle = SDL_CreateWindow(
            "Mayhem",
            x == -1 ? SDL_WINDOWPOS_CENTERED : x,
            y == -1 ? SDL_WINDOWPOS_CENTERED : y,
            static_cast<uint32_t>(window.w * window.sx),
            static_cast<uint32_t>(window.h * window.sy),
            SDL_WINDOW_SHOWN);

        if (window.handle == nullptr) {
            r.error("G001", "unable to create SDL window.");
            return false;
        }

        log_message(
            log_category_t::video,
            "create SDL renderer: accelerated, vsync.");
        window.renderer = SDL_CreateRenderer(
            window.handle,
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if (window.renderer == nullptr) {
            r.error("G001", "unable to create SDL renderer.");
            return false;
        }

        window.texture = SDL_CreateTexture(
            window.renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            window.w,
            window.h);
        log_message(
            log_category_t::video,
            "SDL streaming texture: w={}, h={}",
            window.w,
            window.h);

        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
        SDL_RenderSetLogicalSize(window.renderer, window.w, window.h);
        log_message(
            log_category_t::video,
            "scale quality hint: {}",
            SDL_GetHint(SDL_HINT_RENDER_SCALE_QUALITY));
        log_message(
            log_category_t::video,
            "render logical size: w={}, h={}",
            window.w,
            window.h);

        int32_t wx, wy;
        SDL_GetWindowPosition(window.handle, &wx, &wy);
        window.x = (uint32_t) wx;
        window.y = (uint32_t) wy;
        log_message(
            log_category_t::video,
            "window position: x={}, y={}",
            window.x,
            window.y);
        log_message(
            log_category_t::video,
            "window size (as scaled): w={}, h={}",
            window.w * window.sx,
            window.h * window.sy);

        return true;
    }

    bool window_destroy(common::result& r, window_t& window) {
        SDL_DestroyTexture(window.texture);
        SDL_DestroyRenderer(window.renderer);
        SDL_DestroyWindow(window.handle);
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////

    static const uint8_t* s_keyboard_state = nullptr;
    static bool s_key_pressed[SDL_NUM_SCANCODES];

    bool key_state(uint32_t code) {
        return s_keyboard_state[code];
    }

    bool key_pressed(uint32_t code) {
        for (int8_t i = 0; i < 32; i++) {
            bool pressed = key_state(code);
            if (pressed) {
                if (!s_key_pressed[code]) {
                    s_key_pressed[code] = true;
                }
            } else {
                if (s_key_pressed[code]) {
                    s_key_pressed[code] = false;
                    return true;
                }
            }
        }
        return false;
    }

    bool key_init(common::result& r) {
        if (s_keyboard_state == nullptr)
            s_keyboard_state = SDL_GetKeyboardState(nullptr);
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////

    static uint16_t s_joystick_button_state = 0;

    bool joystick_init(common::result& r) {
        if (s_keyboard_state == nullptr)
            s_keyboard_state = SDL_GetKeyboardState(nullptr);
        return true;
    }

    bool joystick_open(common::result& r, joystick_t& joystick) {
        int32_t number_of_joysticks = SDL_NumJoysticks();
        for (int32_t i = 0; i < number_of_joysticks; ++i) {
            if (SDL_IsGameController(i) == SDL_TRUE) {
                joystick.index = i;
                joystick.controller = SDL_GameControllerOpen(i);
                joystick.name = SDL_GameControllerNameForIndex(i);
                joystick.mapping = SDL_GameControllerMappingForIndex(i);
                return true;
            }
        }
        return false;
    }

    bool joystick_close(common::result& r, joystick_t& joystick) {
        SDL_GameControllerClose(joystick.controller);
        SDL_free((void*)joystick.mapping.data());
        SDL_free((void*)joystick.name.data());
        return true;
    }

    bool joystick_button(joystick_t& joystick, joystick_button_t button) {
        bool button_pressed = false;

        if (joystick.controller != nullptr) {
            button_pressed = SDL_GameControllerGetButton(
                joystick.controller,
                (SDL_GameControllerButton) button) != 0;
        }

        if (!button_pressed) {
            switch (button) {
                case joystick_button_t::a:
                    button_pressed = s_keyboard_state[SDL_SCANCODE_LCTRL];
                    break;
                case joystick_button_t::b:
                    button_pressed = s_keyboard_state[SDL_SCANCODE_RCTRL];
                    break;
                case joystick_button_t::x:
                    button_pressed = s_keyboard_state[SDL_SCANCODE_X];
                    break;
                case joystick_button_t::y:
                    button_pressed = s_keyboard_state[SDL_SCANCODE_Y];
                    break;
                case joystick_button_t::back:
                    button_pressed = s_keyboard_state[SDL_SCANCODE_BACKSPACE];
                    break;
                case joystick_button_t::guide:
                    button_pressed = s_keyboard_state[SDL_SCANCODE_F1];
                    break;
                case joystick_button_t::start:
                    button_pressed = s_keyboard_state[SDL_SCANCODE_RETURN];
                    break;
                case joystick_button_t::left_stick:
                    break;
                case joystick_button_t::right_stick:
                    break;
                case joystick_button_t::left_shoulder:
                    button_pressed = s_keyboard_state[SDL_SCANCODE_LSHIFT];
                    break;
                case joystick_button_t::right_shoulder:
                    button_pressed = s_keyboard_state[SDL_SCANCODE_RSHIFT];
                    break;
                case joystick_button_t::dpad_up:
                    button_pressed = s_keyboard_state[SDL_SCANCODE_UP];
                    break;
                case joystick_button_t::dpad_down:
                    button_pressed = s_keyboard_state[SDL_SCANCODE_DOWN];
                    break;
                case joystick_button_t::dpad_left:
                    button_pressed = s_keyboard_state[SDL_SCANCODE_LEFT];
                    break;
                case joystick_button_t::dpad_right:
                    button_pressed = s_keyboard_state[SDL_SCANCODE_RIGHT];
                    break;
                default:
                    break;
            }
        }

        return button_pressed;
    }

    bool joystick_button_pressed(joystick_t& joystick, joystick_button_t button) {
        const uint32_t bit_mask = (uint32_t)1 << (uint32_t)button;
        for (int8_t i = 0; i < 32; i++) {
            bool pressed = joystick_button(joystick, button);
            if (pressed) {
                if ((s_joystick_button_state & bit_mask) == 0) {
                    s_joystick_button_state |= bit_mask;
                }
            } else {
                if ((s_joystick_button_state & bit_mask) == bit_mask) {
                    s_joystick_button_state &= ~bit_mask;
                    return true;
                }
            }
        }
        return false;
    }

    ///////////////////////////////////////////////////////////////////////////

    bool game_config_load(common::result& r, game_config_t& config) {
        return true;
    }

    bool game_config_save(common::result& r, game_config_t& config) {
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////

    bool timer_update(
            common::result& r,
            entt::registry& registry,
            uint32_t ticks) {
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////

    bool sound_init(common::result& r, sound_system_t& system) {
        auto result = FMOD::System_Create(&system.handle);
        if (result != FMOD_OK) {
            r.error("S001", fmt::format("fmod error {}: {}", result, FMOD_ErrorString(result)));
            return false;
        }

        result = system.handle->init(512, FMOD_INIT_NORMAL, 0);
        if (result != FMOD_OK) {
            r.error("S001", fmt::format("fmod error {}: {}", result, FMOD_ErrorString(result)));
            return false;
        }

        return true;
    }

    bool sound_update(common::result& r, sound_system_t& system) {
        auto result = system.handle->update();
        if (result != FMOD_OK) {
            r.error("S001", fmt::format("fmod error {}: {}", result, FMOD_ErrorString(result)));
            return false;
        }

        return true;
    }

    bool sound_shutdown(common::result& r, sound_system_t& system) {
        auto result = system.handle->release();
        if (result != FMOD_OK) {
            r.error("S001", fmt::format("fmod error {}: {}", result, FMOD_ErrorString(result)));
            return false;
        }

        return true;
    }

    ///////////////////////////////////////////////////////////////////////////

    bool game_run(common::result& r, game_t& game) {
        uint16_t fps = 0;
        uint16_t frame_count = 0;
        uint32_t last_time = SDL_GetTicks();
        uint32_t last_fps_time = last_time;

        while (!SDL_QuitRequested()) {
            uint32_t frame_start_ticks = SDL_GetTicks();

            if (key_pressed(SDL_SCANCODE_ESCAPE)) {
                SDL_Event evt {};
                evt.type = SDL_EventType::SDL_QUIT;
                SDL_PushEvent(&evt);
            }

            if (!sound_update(r, game.sound))
                return false;

            if (!timer_update(r, game.registry, frame_start_ticks))
                return false;
//
//            s_state_context.ticks = frame_start_ticks;
//            state_update(&s_state_context);
//
//            actor_update(frame_start_ticks);
//
//            if (s_show_fps)
//                video_text(white, 2, 2, "FPS: %d", fps);
//
//            video_update(&context->window, frame_start_ticks);

            uint32_t frame_duration = SDL_GetTicks() - frame_start_ticks;

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

        return true;
    }

    bool game_shutdown(common::result& r, game_t& game) {
        if (!sound_shutdown(r, game.sound)) {

        }

        if (!joystick_close(r, game.joystick)) {

        }

        if (!window_destroy(r, game.window)) {

        }

        return true;
    }
}