#include "hill/buffer.hpp"

#include <glad/gl.h>

namespace hill::buffer {
    VertexBuffer::VertexBuffer() {
        glGenBuffers(1, &m_vertex_buffer);
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteBuffers(1, &m_vertex_buffer);
    }

    void VertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer);
    }

    void VertexBuffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::upload_data(const void* vertices, std::size_t size) const {
        glBufferData(GL_ARRAY_BUFFER, long(size), vertices, GL_STATIC_DRAW);  // TODO
    }
}
