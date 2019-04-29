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
#include <string_view>
#include <common/result.h>
#include <SDL_FontCache.h>
#include "game.h"

namespace mayhem {

    struct size_t {
        int32_t w = 0;
        int32_t h = 0;
    };

    struct point_t {
        int32_t x = 0;
        int32_t y = 0;
    };

    enum class tile_flags_t : uint8_t {
        none        = 0b00000000,
        enabled     = 0b00000001,
        hflip       = 0b00000100,
        vflip       = 0b00001000,
        changed     = 0b00010000,
    };

    struct bank_id_t {
        uint8_t bank = 0;
        uint16_t index = 0;
    };

    struct tile_t {
        bank_id_t id{};
        uint8_t palette = 0;
    };

    struct font_t {
        bank_id_t id{};
    };

    enum class layer_flags_t : uint8_t {
        none        = 0b00000000,
        enabled     = 0b00000001,
        changed     = 0b00000010,
    };

    struct layer_t {
        tile_t tile{};
        uint8_t flags = 0;
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
        point_t pos{};
        tile_t tile{};
        uint8_t flags = 0;
    };

    ///////////////////////////////////////////////////////////////////////////

    enum class actor_flags_t : uint8_t {
        none    = 0b00000000,
        enabled = 0b00000001
    };

    struct animation_frame_tile_t {
        tile_t tile{};
        point_t offset{};
        uint8_t flags = 0;
    };

    using animation_frame_tile_list_t = std::vector<animation_frame_tile_t>;

    struct animation_frame_t {
        uint16_t delay = 0;
        animation_frame_tile_list_t tiles{};
    };

    using animation_frame_list_t = std::vector<animation_frame_t>;

    struct animation_t {
        animation_frame_list_t frames{};
    };

    struct actor_t;
    using animation_callback_t = std::function<bool (actor_t*)>;

    struct actor_t {
        point_t pos{};
        uint8_t frame = 0;
        uint8_t flags = 0;
        uint32_t next_tick = 0;
        animation_callback_t animation_callback;
    };

    ///////////////////////////////////////////////////////////////////////////

    struct rect_t {
        point_t pos{};
        size_t size{};
    };

    struct color_t {
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
        uint8_t a = 0;
    };

    struct vline_t {
        color_t color{};
        point_t pos{};
        int32_t h = 0;
    };

    struct hline_t {
        color_t color{};
        point_t pos{};
        int32_t w = 0;
    };

    struct box_t {
        bool fill;
        color_t color{};
        rect_t bounds{};
    };

    struct stamp_t {
        point_t pos;
        tile_t tile;
        uint8_t flags;
    };

    struct blit_t {
        bank_id_t image{};
        rect_t src_rect{};
        rect_t dest_rect{};
    };

    struct text_t {
        point_t pos;
        font_t font;
        color_t color;
        std::string data;
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
        const std::string& path,
        SDL_Renderer* renderer,
        uint32_t size,
        int32_t style,
        color_t color,
        bank_id_t id);

    font_data_t* font_find(bank_id_t id);

    ///////////////////////////////////////////////////////////////////////////

    struct image_t {
        size_t size{};
        int32_t format = 0;
        uint8_t* data = nullptr;
        SDL_Surface* surface = nullptr;
    };

    bool image_load(
        common::result& r,
        const std::string& path,
        int32_t format,
        bank_id_t id);

    image_t* image_find(bank_id_t id);

    ///////////////////////////////////////////////////////////////////////////

    using actor_list_t = std::vector<actor_t>;
    using sprite_list_t = std::vector<sprite_t>;
    using bg_block_list_t = std::vector<bg_block_t>;

    struct video_t {
        rect_t clip{};
        size_t bg_size{};
        size_t tile_size{};
        size_t sprite_size{};
        size_t max_bg_size{};
        uint32_t x_scroll = 0;
        uint32_t y_scroll = 0;
        actor_list_t actors{};
        uint32_t max_sprites{};
        sprite_list_t sprites{};
        bg_block_list_t blocks{};
        SDL_Surface* fg = nullptr;
        SDL_Surface* bg = nullptr;
    };

    struct game_t;

    bool video_queue_text(
        common::result& r,
        game_t& game,
        bank_id_t id,
        color_t color,
        int32_t y,
        int32_t x,
        const std::string& value);

    bool video_queue_image(
        common::result& r,
        game_t& game,
        bank_id_t id,
        int32_t y,
        int32_t x,
        size_t size = {});

    bool video_queue_hline(
        common::result& r,
        game_t& game,
        color_t color,
        int32_t y,
        int32_t x,
        int32_t w);

    bool video_queue_vline(
        common::result& r,
        game_t& game,
        color_t color,
        int32_t y,
        int32_t x,
        int32_t h);

    bool video_queue_box(
        common::result& r,
        game_t& game,
        color_t color,
        int32_t y,
        int32_t x,
        int32_t w,
        int32_t h,
        bool fill = false);

    bool video_init(common::result& r, game_t& game);

    bool video_update(common::result& r, game_t& game);

    bool video_shutdown(common::result& r, game_t& game);

}

