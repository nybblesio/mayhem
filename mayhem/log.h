// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#pragma once

#include <cstdint>
#include <SDL_log.h>
#include <fmt/format.h>

namespace mayhem {

    enum class log_category_t : uint8_t {
        app,
        error,
        assert,
        system,
        audio,
        video,
        renderer,
        input,
        test,
        reserved1,
        reserved2,
        custom
    };

    void log_init();

    template <typename... Args>
    void log_warn(log_category_t category, const std::string& fmt, Args&&... args) {
        SDL_LogWarn(
            static_cast<int>(category),
            "%s",
            fmt::format(fmt, std::forward<Args>(args)...).c_str());
    }

    template <typename... Args>
    void log_debug(log_category_t category, const std::string& fmt, Args&&... args) {
        SDL_LogDebug(
            static_cast<int>(category),
            "%s",
            fmt::format(fmt, std::forward<Args>(args)...).c_str());
    }

    template <typename... Args>
    void log_error(log_category_t category, const std::string& fmt, Args&&... args) {
        SDL_LogError(
            static_cast<int>(category),
            "%s",
            fmt::format(fmt, std::forward<Args>(args)...).c_str());
    }

    template <typename... Args>
    void log_message(log_category_t category, const std::string& fmt, Args&&... args) {
        SDL_LogMessage(
            static_cast<int>(category),
            SDL_LOG_PRIORITY_INFO,
            "%s",
            fmt::format(fmt, std::forward<Args>(args)...).c_str());
    }

    template <typename... Args>
    void log_critical(log_category_t category, const std::string& fmt, Args&&... args) {
        SDL_LogCritical(
            static_cast<int>(category),
            "%s",
            fmt::format(fmt, std::forward<Args>(args)...).c_str());
    }

}

