// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#include <fmt/format.h>
#include <mayhem/game.h>

static void print_results(const mayhem::common::result& r) {
    auto has_messages = !r.messages().empty();

    if (has_messages)
        fmt::print("\n");

    auto messages = r.messages();
    for (size_t i = 0; i < messages.size(); i++) {
        const auto& msg = messages[i];
        fmt::print(
            "[{}] {}{}\n",
            msg.code(),
            msg.is_error() ? "ERROR: " : "WARNING: ",
            msg.message());
        if (!msg.details().empty()) {
            fmt::print("{}\n", msg.details());
        }
        if (i < messages.size() - 1)
            fmt::print("\n");
    }
}


int main(int argc, const char** argv) {
    mayhem::game_t game{};
    mayhem::common::result result{};

    defer(print_results(result));

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