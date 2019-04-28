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

namespace mayhem {

    enum class sprite_flags_t : uint8_t {
        none        = 0b00000000,
        enabled     = 0b00000001,
        collided    = 0b00000010,
        hflip       = 0b00000100,
        vflip       = 0b00001000,
        changed     = 0b00010000,
    };

    enum class tile_flags_t : uint8_t {
        none        = 0b00000000,
        enabled     = 0b00000001,
        hflip       = 0b00000100,
        vflip       = 0b00001000,
        changed     = 0b00010000,
    };

    struct tile_t {
        uint8_t bank;
        uint16_t index;
        uint8_t palette;
    };

    struct sprite_t {
        int32_t x;
        int32_t y;
        tile_t tile;
        uint8_t flags;
    };

    enum class layer_flags_t : uint8_t {
        none        = 0b00000000,
        enabled     = 0b00000001,
    };

    struct layer_t {
        tile_t tile;
        uint8_t flags;
    };

    using layer_list_t = std::vector<layer_t>;

    struct map_entry_t {
        layer_list_t layers{};
    };

    enum class actor_flags_t : uint8_t {
        none    = 0b00000000,
        enabled = 0b00000001
    };

    struct animation_frame_tile_t {
        tile_t tile;
        uint8_t flags;
        int16_t x_offset;
        int16_t y_offset;
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
        int32_t x;
        int32_t y;
        uint8_t frame;
        uint8_t flags;
        uint32_t next_tick;
        animation_callback_t animation_callback;
    };

}
