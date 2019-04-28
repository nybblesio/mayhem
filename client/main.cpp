// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#include <mayhem/game.h>

int main(int argc, const char** argv) {
    mayhem::game_t game{};
    mayhem::common::result result{};

    if (!mayhem::game_init(result, game)) {
        return 1;
    }

    if (!mayhem::game_run(result, game)) {
        return 1;
    }

    if (!mayhem::game_shutdown(result, game)) {
        return 1;
    }

    return 0;
}