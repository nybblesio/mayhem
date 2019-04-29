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

    using namespace std::literals;

    class boot_state : public state {
    public:
        static constexpr uint32_t type = 0x1a;

        explicit boot_state(state_machine* machine);

        std::string_view name() const override {
            return "boot"sv;
        }

        bool update(common::result& r, game_t& game) override;

    private:
    };

}

