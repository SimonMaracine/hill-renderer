#pragma once

#include <cstddef>

#include "hill/primitives/common.hpp"

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
        void upload_data(const void* vertices, std::size_t size, common::BufferUsage usage = common::BufferUsage::StaticDraw) const;
    private:
        unsigned int m_vertex_buffer {};
    };
}
