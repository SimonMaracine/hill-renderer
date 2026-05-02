#pragma once

#include <string>
#include <filesystem>

#include "hill/error.hpp"

namespace hill::utility {
    using Buffer = std::string;

    void read_file(const std::filesystem::path& path, Buffer& buffer);
    void write_file(const std::filesystem::path& path, const Buffer& buffer);

    using FilePath = std::filesystem::path;

    struct FileError : error::Error {
        using Error::Error;
    };
}
