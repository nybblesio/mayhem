// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#include "editor_state.h"
#include "game.h"

namespace mayhem {

    editor_state::editor_state(state_machine* machine) : state(machine) {
    }

    bool editor_state::enter(common::result& r, game_t& game) {
        game.in_editor = true;
        return true;
    }

    bool editor_state::leave(common::result& r, game_t& game) {
        return true;
    }

    bool editor_state::update(common::result& r, game_t& game) {
        video_queue_box(r, game, color_t{0x20, 0xd6, 0xc7}, 100, 100, 256, 64, true);
        video_queue_box(r, game, color_t{0x8b, 0x93, 0xaf}, 0, 0, 512, 480, true);
        return true;
    }

}