#pragma once

namespace hill::buffer {
    enum class Usage {
        StaticDraw,
        StaticRead,
        StaticCopy,
        DynamicDraw,
        DynamicRead,
        DynamicCopy,
        StreamDraw,
        StreamRead,
        StreamCopy
    };

    unsigned int usage_enum(Usage usage);
}
