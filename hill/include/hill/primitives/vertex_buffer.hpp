#pragma once

#include <cstddef>

#include "buffer.hpp"

namespace hill::vertex_buffer {
    class VertexBuffer {
    public:
        VertexBuffer();
        ~VertexBuffer();

        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer(VertexBuffer&&) = delete;
        VertexBuffer& operator=(VertexBuffer&&) = delete;

        unsigned int id() const { return m_vertex_buffer; }

        void bind() const;
        void unbind() const;
        void upload_data(const void* vertices, std::size_t size, buffer::Usage usage = buffer::Usage::StaticDraw) const;
    private:
        unsigned int m_vertex_buffer {};
    };
}
