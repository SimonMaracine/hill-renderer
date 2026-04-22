#include "hill/primitives/common.hpp"

#include <utility>

#include <glad/gl.h>

namespace hill::common {
    unsigned int buffer_usage_enum(BufferUsage buffer_usage) {
        switch (buffer_usage) {
            case BufferUsage::StaticDraw:
                return GL_STATIC_DRAW;
            case BufferUsage::StaticRead:
                return GL_STATIC_READ;
            case BufferUsage::StaticCopy:
                return GL_STATIC_COPY;
            case BufferUsage::DynamicDraw:
                return GL_DYNAMIC_DRAW;
            case BufferUsage::DynamicRead:
                return GL_DYNAMIC_READ;
            case BufferUsage::DynamicCopy:
                return GL_DYNAMIC_COPY;
            case BufferUsage::StreamDraw:
                return GL_STREAM_DRAW;
            case BufferUsage::StreamRead:
                return GL_STREAM_READ;
            case BufferUsage::StreamCopy:
                return GL_STREAM_COPY;
        }

        std::unreachable();
    }
}
