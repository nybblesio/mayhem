// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#include <string>
#include <fmt/format.h>
#include "log.h"
#include "state_machine.h"

namespace mayhem {

    using namespace std::literals;

    state::state(state_machine* machine) : _machine(machine) {
    }

    state_machine* state::machine() {
        return _machine;
    }

    std::string_view state::name() const {
        return "default"sv;
    }

    bool state::enter(common::result& r, game_t& game) {
        return true;
    }

    bool state::leave(common::result& r, game_t& game) {
        return true;
    }

    bool state::update(common::result& r, game_t& game) {
        return true;
    }

    ///////////////////////////////////////////////////////////////////////////

    state_machine::~state_machine() {
        for (auto& kvp : _storage)
            delete kvp.second;
    }

    state* state_machine::top() const {
        if (_states.empty())
            return nullptr;
        return _states.top();
    }

    std::size_t state_machine::depth() const {
        return _states.size();
    }

    state* state_machine::find_state(uint32_t type) {
        auto it = _storage.find(type);
        if (it == std::end(_storage))
            return nullptr;
        return it->second;
    }

    bool state_machine::pop(common::result& r, game_t& game) {
        if (_states.empty()) {
            r.error("G400", "the game state stack must not be empty.");
            return false;
        }
        auto top = _states.top();
        log_debug(log_category_t::app, "leave state: {}", top->name());
        if (!top->leave(r, game))
            return false;
        _states.pop();
        if (_states.empty()) {
            r.error("G400", "the game state stack must not be empty.");
            return false;
        }
        top = _states.top();
        log_debug(log_category_t::app, "enter state: {}", top->name());
        return top->enter(r, game);
    }

    bool state_machine::update(common::result& r, game_t& game) {
        if (_states.empty()) {
            r.error("G400", "the game state stack must not be empty.");
            return false;
        }

        auto top = _states.top();
        return top->update(r, game);
    }

    bool state_machine::push(common::result& r, game_t& game, uint32_t type) {
        auto state = find_state(type);
        if (state == nullptr) {
            r.error("G401", fmt::format("state not found: {}", type));
            return false;
        }

        if (!_states.empty()) {
            auto top = _states.top();
            log_debug(log_category_t::app, "leave state: {}", top->name());
            if (!top->leave(r, game))
                return false;
        }

        _states.push(state);
        log_debug(log_category_t::app, "enter state: {}", state->name());
        return state->enter(r, game);
    }

}