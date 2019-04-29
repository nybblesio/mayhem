// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#pragma once

#include <vector>
#include <cstdint>
#include <functional>
#include <filesystem>
#include <string_view>
#include <common/result.h>
#include <SDL_FontCache.h>

namespace mayhem {

    struct size_t {
        uint32_t w, h;
    };

    struct point_t {
        int32_t x, y;
    };

    enum class tile_flags_t : uint8_t {
        none        = 0b00000000,
        enabled     = 0b00000001,
        hflip       = 0b00000100,
        vflip       = 0b00001000,
        changed     = 0b00010000,
    };

    struct bank_id_t {
        uint8_t bank;
        uint16_t index;
    };

    struct tile_t {
        bank_id_t id;
        uint8_t palette;
    };

    struct font_t {
        bank_id_t id;
    };

    enum class layer_flags_t : uint8_t {
        none        = 0b00000000,
        enabled     = 0b00000001,
        changed     = 0b00000010,
    };

    struct layer_t {
        tile_t tile;
        uint8_t flags;
    };

    using layer_list_t = std::vector<layer_t>;

    struct bg_block_t {
        layer_list_t layers{};
    };

    ///////////////////////////////////////////////////////////////////////////

    enum class sprite_flags_t : uint8_t {
        none        = 0b00000000,
        enabled     = 0b00000001,
        collided    = 0b00000010,
        hflip       = 0b00000100,
        vflip       = 0b00001000,
        changed     = 0b00010000,
    };

    struct sprite_t {
        point_t pos;
        tile_t tile;
        uint8_t flags;
    };

    ///////////////////////////////////////////////////////////////////////////

    enum class actor_flags_t : uint8_t {
        none    = 0b00000000,
        enabled = 0b00000001
    };

    struct animation_frame_tile_t {
        tile_t tile;
        uint8_t flags;
        point_t offset;
    };

    using animation_frame_tile_list_t = std::vector<animation_frame_tile_t>;

    struct animation_frame_t {
        uint16_t delay;
        animation_frame_tile_list_t tiles{};
    };

    using animation_frame_list_t = std::vector<animation_frame_t>;

    struct animation_t {
        animation_frame_list_t frames{};
    };

    struct actor_t;
    using animation_callback_t = std::function<bool (actor_t*)>;

    struct actor_t {
        point_t pos;
        uint8_t frame;
        uint8_t flags;
        uint32_t next_tick;
        animation_callback_t animation_callback;
    };

    ///////////////////////////////////////////////////////////////////////////

    struct rect_t {
        point_t pos;
        size_t size;
    };

    struct color_t {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };

    struct vline_t {
        color_t color;
        point_t pos;
        int32_t h;
    };

    struct hline_t {
        color_t color;
        point_t pos;
        int32_t w;
    };

    struct box_t {
        bool fill;
        color_t color;
        rect_t bounds;
    };

    struct stamp_t {
        point_t pos;
        tile_t tile;
        uint8_t flags;
    };

    struct text_t {
        point_t pos;
        font_t font;
        color_t color;
        std::string_view data;
    };

    ///////////////////////////////////////////////////////////////////////////

    static constexpr uint8_t system_bank = 0xff;

    struct font_data_t {
        uint32_t size;
        color_t color;
        int32_t style;
        FC_Font* handle = nullptr;
    };

    bool font_load(
        common::result& r,
        const std::filesystem::path& path,
        SDL_Renderer* renderer,
        uint32_t size,
        int32_t style,
        color_t color,
        bank_id_t id);

    font_data_t* font_find(bank_id_t id);

    ///////////////////////////////////////////////////////////////////////////

    using actor_list_t = std::vector<actor_t>;
    using sprite_list_t = std::vector<sprite_t>;
    using bg_block_list_t = std::vector<bg_block_t>;

    struct video_t {
        size_t bg_size;
        size_t tile_size;
        size_t sprite_size;
        size_t max_bg_size;
        uint32_t max_sprites;
        uint32_t x_scroll = 0;
        uint32_t y_scroll = 0;
        actor_list_t actors{};
        sprite_list_t sprites{};
        bg_block_list_t blocks{};
        SDL_Surface* fg = nullptr;
        SDL_Surface* bg = nullptr;
    };

    struct game_t;

    bool video_init(common::result& r, game_t& game);

    bool video_update(common::result& r, game_t& game);

    bool video_shutdown(common::result& r, game_t& game);

}

