// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#pragma once

#include "state_machine.h"

namespace mayhem {

    class editor_state : public state {
    public:
        static constexpr uint32_t type = 0x1b;

        explicit editor_state(state_machine* machine);

        bool enter(common::result& r, game_t& game) override;

        bool leave(common::result& r, game_t& game) override;

        bool update(common::result& r, game_t& game) override;

    private:
    };

}

