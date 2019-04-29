// ----------------------------------------------------------------------------
//
// Mayhem
//
// Copyright (C) 2019 Jeff Panici
// All rights reserved.
//
// ----------------------------------------------------------------------------

#include "bank_manager.h"
 #include <utility>
namespace mayhem {

    bank_entry::bank_entry(
        std::string name,
        uint8_t type,
        size_t size) : _name(std::move(name)),
                       _type(type),
                       _size(size) {
    }

    uint8_t* bank_entry::data() {
        return _data.release();
    }

    size_t bank_entry::size() const {
        return _size;
    }

    uint8_t bank_entry::type() const {
        return _type;
    }

    std::string_view bank_entry::name() const {
        return std::string_view(_name);
    }

    uint8_t* bank_entry::allocate_data(size_t size) {
        _data.reset(new uint8_t[size]);
        _size = size;
        return _data.release();
    }

    bank_entry* bank::find(const std::string& name) {
        auto it = _entries.find(name);
        if (it == std::end(_entries))
            return nullptr;
        return &it->second;
    }

    ///////////////////////////////////////////////////////////////////////////

    bool bank::empty() const {
        return _entries.empty();
    }

    size_t bank::size() const {
        return _entries.size();
    }

    ///////////////////////////////////////////////////////////////////////////

    bank_file::bank_file(std::string path) : _path(std::move(path)) {
    }

    bool bank_file::empty() const {
        return _banks.empty();
    }

    size_t bank_file::size() const {
        return _banks.size();
    }

    std::string_view bank_file::path() const {
        return std::string_view(_path);
    }

    ///////////////////////////////////////////////////////////////////////////

    bank_manager::~bank_manager() {
    }

    bool bank_manager::empty() const {
        return _files.empty();
    }

    bool bank_manager::exists(const std::string& path) {
        return _files.count(path) > 0;
    }

    bank_file* bank_manager::find(const std::string& path) {
        auto it = _files.find(path);
        if (it == std::end(_files))
            return nullptr;
        return &it->second;
    }

    bool bank_manager::save(common::result& r, bank_file* file) {
        return false;
    }

    bank_file* bank_manager::load(common::result& r, const std::string& path) {
        return nullptr;
    }

}