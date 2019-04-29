// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#pragma once

#include <cstdint>
#include <SDL_mouse.h>
#include <common/result.h>
#include <SDL_gamecontroller.h>

namespace mayhem {

    bool key_state(uint32_t code);

    bool key_pressed(uint32_t code);

    bool key_init(common::result& r);

    ///////////////////////////////////////////////////////////////////////////

    enum class mouse_button_t : int8_t {
        left,
        right,
    };

    struct mouse_position_t {
        int32_t x, y;
    };

    mouse_position_t mouse_position();

    bool mouse_button(mouse_button_t button);

    bool mouse_button_pressed(mouse_button_t button);

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

}

