// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#include <cstdarg>
#include "log.h"

namespace mayhem {

    void log_init() {
        SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);
    }

}