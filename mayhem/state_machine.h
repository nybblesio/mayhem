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

    class state_machine;

    class state {
    public:
        explicit state(state_machine* machine);

        virtual std::string_view name() const;

        virtual bool enter(common::result& r);

        virtual bool leave(common::result& r);

        virtual bool update(common::result& r, uint32_t ticks);

    protected:
        state_machine* machine();

    private:
        state_machine* _machine = nullptr;
    };

    ///////////////////////////////////////////////////////////////////////////

    class state_machine {
    public:
        state_machine();

        state_machine(const state_machine&) = delete;

        ~state_machine();

        state* top() const;

        bool pop(common::result& r);

        state* find_state(uint32_t type);

        bool push(common::result& r, uint32_t type);

        bool update(common::result& r, uint32_t ticks);

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

    private:
        std::stack<state*> _states {};
        std::unordered_map<uint32_t, state*> _storage {};
    };

}

