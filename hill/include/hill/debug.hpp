#pragma once

#include <string>

namespace hill::debug {
    using OutputCallback = void(*)(std::string);

    struct DebugOutput {
        OutputCallback output_callback {};
        bool ignore_notification = true;
    };

    void initialize(DebugOutput debug_output);
}
