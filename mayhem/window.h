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
#include <cstdint>
#include <common/result.h>

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

}

