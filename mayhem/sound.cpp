// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#include <fmt/format.h>
#include <fmod_errors.h>
#include "sound.h"

namespace mayhem {

    bool sound_init(common::result& r, sound_system_t& system) {
        auto result = FMOD::System_Create(&system.handle);
        if (result != FMOD_OK) {
            r.error("S001", fmt::format("fmod error {}: {}", result, FMOD_ErrorString(result)));
            return false;
        }

        result = system.handle->init(512, FMOD_INIT_NORMAL, 0);
        if (result != FMOD_OK) {
            r.error("S001", fmt::format("fmod error {}: {}", result, FMOD_ErrorString(result)));
            return false;
        }

        return true;
    }

    bool sound_update(common::result& r, sound_system_t& system) {
        auto result = system.handle->update();
        if (result != FMOD_OK) {
            r.error("S001", fmt::format("fmod error {}: {}", result, FMOD_ErrorString(result)));
            return false;
        }

        return true;
    }

    bool sound_shutdown(common::result& r, sound_system_t& system) {
        auto result = system.handle->release();
        if (result != FMOD_OK) {
            r.error("S001", fmt::format("fmod error {}: {}", result, FMOD_ErrorString(result)));
            return false;
        }

        return true;
    }

}