// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#include <fmt/format.h>
#include <unordered_map>
#include <SDL_surface.h>
#include <SDL_FontCache.h>
#include "game.h"
#include "video.h"
#include "window.h"

namespace mayhem {

    static std::unordered_map<std::string, font_data_t> s_fonts{};

    static std::string make_font_key(bank_id_t id) {
        return fmt::format("{}:{}", id.bank, id.index);
    }

    static SDL_Color to_fc_color(const color_t& color) {
        return FC_MakeColor(color.r, color.g, color.b, color.a);
    }

    ///////////////////////////////////////////////////////////////////////////

    bool font_load(
            common::result& r,
            const std::filesystem::path& path,
            SDL_Renderer* renderer,
            uint16_t size,
            uint32_t style,
            color_t color,
            bank_id_t id) {
        auto font = font_find(id);
        if (font != nullptr)
            return true;

        auto result = s_fonts.insert(std::make_pair(make_font_key(id), font_data_t {}));
        if (!result.second)
            return false;

        auto& data = result.first->second;
        data.size = size;
        data.style = style;
        data.color = color;
        data.handle = FC_CreateFont();

        return FC_LoadFont(
            data.handle,
            renderer,
            path.c_str(),
            size,
            to_fc_color(data.color),
            style) != 0;
    }

    font_data_t* font_find(bank_id_t id) {
        auto it = s_fonts.find(make_font_key(id));
        if (it == std::end(s_fonts))
            return nullptr;
        return &it->second;
    }

    ///////////////////////////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////////////////////////

    bool video_init(common::result& r, game_t& game) {
        game.video.sprites.reserve(game.video.max_sprites);
        game.video.blocks.reserve(game.video.max_bg_size.w * game.video.max_bg_size.h);

        const auto bg_surface_width = game.video.tile_size.w * game.video.bg_size.w;
        const auto bg_surface_height = game.video.tile_size.h * game.video.bg_size.h;
        game.video.bg = SDL_CreateRGBSurfaceWithFormat(
            0,
            bg_surface_width,
            bg_surface_height,
            32,
            SDL_PIXELFORMAT_RGBA8888);
        SDL_SetSurfaceBlendMode(game.video.bg, SDL_BLENDMODE_NONE);

        game.video.fg = SDL_CreateRGBSurfaceWithFormat(0,
            screen_width,
            screen_height,
            32,
            SDL_PIXELFORMAT_RGBA8888);
        SDL_SetSurfaceBlendMode(game.video.fg, SDL_BLENDMODE_NONE);
        SDL_SetSurfaceRLE(game.video.fg, SDL_TRUE);

        return true;
    }

    bool video_update(common::result& r, game_t& game) {
        const auto source_rect = SDL_Rect {
            (int)game.video.x_scroll,
            (int)game.video.y_scroll,
            screen_width,
            screen_height
        };
        SDL_BlitSurface(
            game.video.bg,
            &source_rect,
            game.video.fg,
            nullptr);
        SDL_UpdateTexture(
            game.window.texture,
            nullptr,
            game.video.fg->pixels,
            game.video.fg->pitch);
        SDL_RenderCopy(
            game.window.renderer,
            game.window.texture,
            nullptr,
            nullptr);

        SDL_RenderPresent(game.window.renderer);
        return true;
    }

    bool video_shutdown(common::result& r, game_t& game) {
        SDL_FreeSurface(game.video.bg);
        SDL_FreeSurface(game.video.fg);

        for (const auto& kvp : s_fonts)
            FC_FreeFont(kvp.second.handle);

        return true;
    }

}