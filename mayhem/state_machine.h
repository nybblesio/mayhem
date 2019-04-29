// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#pragma once

#include <stack>
#include <unordered_map>
#include <common/result.h>

namespace mayhem {

    struct game_t;
    class state_machine;

    class state {
    public:
        explicit state(state_machine* machine);

        virtual std::string_view name() const;

        virtual bool enter(common::result& r, game_t& game);

        virtual bool leave(common::result& r, game_t& game);

        virtual bool update(common::result& r, game_t& game);

    protected:
        state_machine* machine();

    private:
        state_machine* _machine = nullptr;
    };

    ///////////////////////////////////////////////////////////////////////////

    class state_machine {
    public:
        state_machine() = default;

        state_machine(const state_machine&) = delete;

        ~state_machine();

        state* top() const;

        state* find_state(uint32_t type);

        bool pop(common::result& r, game_t& game);

        bool update(common::result& r, game_t& game);

        template <typename T, typename... Args>
        bool register_state(uint32_t type, Args&&... args) {
            static_assert(
                std::is_base_of<state, T>::value,
                "T must be a subclass of state");
            auto result = _storage.insert(std::make_pair(
                type,
                new T(this, std::forward<Args>(args)...)));
            return result.second;
        }

        bool push(common::result& r, game_t& game, uint32_t type);

    private:
        std::stack<state*> _states {};
        std::unordered_map<uint32_t, state*> _storage {};
    };

}

