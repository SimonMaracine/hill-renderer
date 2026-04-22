#pragma once

#include <memory>
#include <cstddef>

namespace hill::common {
    template<typename Primitive>
    struct IdHash {
        std::size_t operator()(const std::shared_ptr<Primitive>& primitive) const {
            const auto id = primitive->id();
            return std::hash<std::remove_const_t<decltype(id)>>()(id);
        }
    };

    enum class BufferUsage {
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

    unsigned int buffer_usage_enum(BufferUsage buffer_usage);
}
