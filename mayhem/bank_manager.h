// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#pragma once

#include <map>
#include <vector>
#include <string>
#include <memory>
#include <cstdint>
#include <filesystem>
#include <unordered_map>
#include <common/result.h>

namespace mayhem {

    struct bank_index_entry_t {
        uint8_t bank;
        uint8_t type;
        char name[22];
        uint32_t offset;
        uint32_t size;
    };

    struct bank_index_t {
        uint16_t size;
        uint16_t used_count;
        bank_index_entry_t* entries = nullptr;
    };

    struct bank_file_t {
        char magic[4];
        bank_index_t head_index;
    };

    ///////////////////////////////////////////////////////////////////////////

    class bank_entry {
    public:
        bank_entry(
            std::string name,
            uint8_t type,
            size_t size);

        uint8_t* data();

        size_t size() const;

        uint8_t type() const;

        std::string_view name() const;

        uint8_t* allocate_data(size_t size);

    private:
        size_t _size;
        uint8_t _type;
        std::string _name;
        std::unique_ptr<uint8_t> _data;
    };

    ///////////////////////////////////////////////////////////////////////////

    class bank {
    public:
        bank() = default;

        bool empty() const;

        size_t size() const;

        bank_entry* find(const std::string& name);

    private:
        std::map<std::string, bank_entry> _entries{};
    };

    using bank_list_t = std::vector<bank>;

    ///////////////////////////////////////////////////////////////////////////

    class bank_file {
    public:
        explicit bank_file(std::filesystem::path path);

        bool empty() const;

        size_t size() const;

        const std::filesystem::path& path() const;

    private:
        bank_list_t _banks{};
        std::filesystem::path _path;
    };

    ///////////////////////////////////////////////////////////////////////////

    class bank_manager {
    public:
        bank_manager() = default;

        ~bank_manager();

        bool empty() const;

        bool save(common::result& r, bank_file* file);

        bool exists(const std::filesystem::path& path);

        bank_file* find(const std::filesystem::path& path);

        bank_file* load(common::result& r, const std::filesystem::path& path);

    private:
        std::unordered_map<std::string, bank_file> _files{};
    };

}

