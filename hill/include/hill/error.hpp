#pragma once

#include <stdexcept>

namespace hill {
    struct Error : std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}
