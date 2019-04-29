// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#include "timer.h"

namespace mayhem {

    bool timer_start(
            common::result& r,
            game_t& game,
            uint32_t duration,
            const timer_t::timer_callback_t& callback,
            id entity_id) {
        auto stand_alone = false;
        if (entity_id == entt::null) {
            entity_id = game.registry.create();
            stand_alone = true;
        }

        auto& timer = game.registry.assign<timer_t>(entity_id);
        timer.active = true;
        timer.duration = duration;
        timer.callback = callback;
        timer.stand_alone = stand_alone;
        timer.expiry = game.ticks + duration;

        return true;
    }

    bool timer_update(common::result& r, game_t& game) {
        auto view = game.registry.view<timer_t>();
        for (auto entity : view) {
            timer_t& timer = view.get(entity);

            if (!timer.active)
                continue;

            if (game.ticks > timer.expiry) {
                if (timer.callback == nullptr) {
                    if (timer.stand_alone) {
                        game.registry.destroy(entity);
                    } else {
                        timer.active = false;
                    }
                    continue;
                }

                auto kill = !timer.callback(&timer, game);
                if (kill) {
                    if (timer.stand_alone) {
                        game.registry.destroy(entity);
                    } else {
                        timer.active = false;
                    }
                } else {
                    timer.expiry = game.ticks + timer.duration;
                }
            }
        }
        return true;
    }

}