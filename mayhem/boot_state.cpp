// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#include "game.h"
#include "boot_state.h"

namespace mayhem {

    boot_state::boot_state(state_machine* machine) : state(machine) {
    }

    bool boot_state::update(common::result& r, game_t& game) {
        video_queue_image(r, game, bank_id_t{0xff, 2}, 100, 135);
        video_queue_image(r, game, bank_id_t{0xff, 1}, 400, 175);
        return true;
    }

}