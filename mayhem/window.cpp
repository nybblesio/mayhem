// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#include "log.h"
#include "window.h"

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

}