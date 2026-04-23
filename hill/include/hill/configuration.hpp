#pragma once

#include <optional>

#include "hill/debug.hpp"

namespace hill::configuration {
    struct Configuration {
        std::optional<debug::DebugOutput> debug_output;
    };
}
