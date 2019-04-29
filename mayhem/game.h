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
#include <common/result.h>
#include <entt/entity/registry.hpp>
#include "log.h"
#include "video.h"
#include "input.h"
#include "sound.h"
#include "window.h"
#include "bank_manager.h"
#include "state_machine.h"

namespace mayhem {

    using id = entt::registry::entity_type;

    struct game_config_t {
        bool show_fps = true;
        int32_t window_x = -1;
        int32_t window_y = -1;
    };

    bool game_config_load(common::result& r, game_config_t& config);

    bool game_config_save(common::result& r, game_config_t& config);

    ///////////////////////////////////////////////////////////////////////////

    struct game_t {
        uint32_t ticks;
        video_t video{};
        window_t window{};
        bank_manager banks{};
        joystick_t joystick{};
        game_config_t config{};
        sound_system_t sound{};
        bool in_editor = false;
        entt::registry registry{};
    };

    bool game_run(common::result& r, game_t& game);

    bool game_init(common::result& r, game_t& game);

    bool game_shutdown(common::result& r, game_t& game);

}

