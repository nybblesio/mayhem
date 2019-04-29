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
#include <functional>
#include <common/result.h>
#include "game.h"

namespace mayhem {

    struct timer_t {
        using timer_callback_t = std::function<bool (timer_t*, game_t&)>;

        bool active;
        uint32_t expiry;
        bool stand_alone;
        uint32_t duration;
        timer_callback_t callback;
    };

    bool timer_start(
        common::result& r,
        game_t& game,
        uint32_t duration,
        const timer_t::timer_callback_t& callback,
        id entity_id = entt::null);

    bool timer_update(common::result& r, game_t& game);

}

