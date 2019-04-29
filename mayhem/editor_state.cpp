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
        return true;
    }

}