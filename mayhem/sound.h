// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#pragma once

#include <fmod.hpp>
#include <common/result.h>

namespace mayhem {

    struct sound_system_t {
        FMOD::System* handle = nullptr;
    };

    bool sound_init(common::result& r, sound_system_t& system);

    bool sound_update(common::result& r, sound_system_t& system);

    bool sound_shutdown(common::result& r, sound_system_t& system);

}

