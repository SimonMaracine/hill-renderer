#pragma once

#include <stdexcept>

namespace hill::error {
    struct Error : std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}
