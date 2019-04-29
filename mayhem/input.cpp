// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#include <SDL_keyboard.h>
#include "input.h"

namespace mayhem {

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

}