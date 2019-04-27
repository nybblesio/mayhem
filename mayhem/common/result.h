// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#pragma once

#include <string>
#include <vector>
#include <utility>
#include "result_message.h"

namespace mayhem::common {

    class result {
    public:
        result() = default;

        inline void fail() {
            _success = false;
        }

        inline void succeed() {
            _success = true;
        }

        inline void info(
                const std::string& code,
                const std::string& message,
                const std::string& details = {}) {
            _messages.emplace_back(
                code,
                message,
                details,
                result_message::types::info);
        }

        inline void error(
                const std::string& code,
                const std::string& message,
                const std::string& details = {}) {
            _messages.emplace_back(
                    code,
                    message,
                    details,
                    result_message::types::error);
            fail();
        }

        inline void warning(
                const std::string& code,
                const std::string& message,
                const std::string& details = {}) {
            _messages.emplace_back(
                code,
                message,
                details,
                result_message::types::warning);
        }

        inline bool is_failed() const {
            return !_success;
        }

        void remove_code(const std::string& code) {
            for (auto it = _messages.begin(); it != _messages.end(); ++it) {
                if ((*it).code() == code)
                    it = _messages.erase(it);
            }
        }

        inline const result_message_list& messages() const {
            return _messages;
        }

        inline bool has_code(const std::string& code) const {
            for (const auto& msg : _messages)
                if (msg.code() == code)
                    return true;
            return false;
        }

        inline const result_message* find_code(const std::string& code) const {
            for (auto it = _messages.begin(); it != _messages.end(); ++it) {
                if ((*it).code() == code)
                    return &(*it);
            }
            return nullptr;
        }

    private:
        bool _success = true;
        result_message_list _messages {};
    };

}
