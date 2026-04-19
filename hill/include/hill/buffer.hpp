#pragma once

#include <cstddef>

namespace hill::buffer {
    class VertexBuffer {
    public:
        VertexBuffer();
        ~VertexBuffer();

        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer(VertexBuffer&&) = delete;
        VertexBuffer& operator=(VertexBuffer&&) = delete;

        unsigned int handle() const { return m_vertex_buffer; }

        void bind() const;
        void unbind() const;
        void upload_data(const void* vertices, std::size_t size) const;
    private:
        unsigned int m_vertex_buffer {};
    };
}
