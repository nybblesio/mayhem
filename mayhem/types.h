// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#pragma once

#include <SDL.h>
#include <string>
#include <cstdint>
#include <fmod.hpp>
#include <string_view>
#include <common/rune.h>
#include <common/bytes.h>
#include <common/defer.h>
#include <common/result.h>
#include <common/string_support.h>
#include <entt/entity/registry.hpp>
#include "log.h"

namespace mayhem {

    static constexpr uint32_t scale_x = 2;
    static constexpr uint32_t scale_y = 2;
    static constexpr uint32_t screen_width = 512;
    static constexpr uint32_t screen_height = 480;
    static constexpr uint32_t target_frame_rate = 60;
    static constexpr uint32_t milliseconds_per_frame = 1000 / target_frame_rate;

    struct window_t {
        int32_t x, y;
        uint32_t w, h;
        uint32_t sx, sy;
        SDL_Window* handle = nullptr;
        SDL_Surface* surface = nullptr;
        SDL_Texture* texture = nullptr;
        SDL_Renderer* renderer = nullptr;
    };

    bool window_destroy(common::result& r, window_t& window);

    bool window_create(common::result& r, window_t& window, int32_t x, int32_t y);

    ///////////////////////////////////////////////////////////////////////////

    struct game_config_t {
        int32_t window_x = -1;
        int32_t window_y = -1;
    };

    bool game_config_load(common::result& r, game_config_t& config);

    bool game_config_save(common::result& r, game_config_t& config);

    ///////////////////////////////////////////////////////////////////////////

    bool key_state(uint32_t code);

    bool key_pressed(uint32_t code);

    bool key_init(common::result& r);

    ///////////////////////////////////////////////////////////////////////////

    enum class joystick_button_t : int8_t {
        invalid = -1,
        a,
        b,
        x,
        y,
        back,
        guide,
        start,
        left_stick,
        right_stick,
        left_shoulder,
        right_shoulder,
        dpad_up,
        dpad_down,
        dpad_left,
        dpad_right,
    };

    struct joystick_t {
        int32_t index;
        std::string_view name;
        std::string_view mapping;
        SDL_GameController* controller = nullptr;
    };

    bool joystick_init(common::result& r);

    bool joystick_open(common::result& r, joystick_t& joystick);

    bool joystick_close(common::result& r, joystick_t& joystick);

    bool joystick_button(joystick_t& joystick, joystick_button_t button);

    bool joystick_button_pressed(joystick_t& joystick, joystick_button_t button);

    ///////////////////////////////////////////////////////////////////////////

    bool timer_update(
        common::result& r,
        entt::registry& registry,
        uint32_t ticks);

    ///////////////////////////////////////////////////////////////////////////

    struct sound_system_t {
        FMOD::System* handle = nullptr;
    };

    bool sound_init(common::result& r, sound_system_t& system);

    bool sound_update(common::result& r, sound_system_t& system);

    bool sound_shutdown(common::result& r, sound_system_t& system);

    ///////////////////////////////////////////////////////////////////////////

    struct game_t {
        window_t window{};
        game_config_t config{};
        joystick_t joystick{};
        sound_system_t sound{};
        entt::registry registry{};
    };

    bool game_run(common::result& r, game_t& game);

    bool game_init(common::result& r, game_t& game);

    bool game_shutdown(common::result& r, game_t& game);

}