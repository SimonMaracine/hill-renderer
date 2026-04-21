#include "hill/primitives/element_buffer.hpp"

#include <glad/gl.h>

namespace hill::element_buffer {
    ElementBuffer::ElementBuffer() {
        glGenBuffers(1, &m_element_buffer);
    }

    ElementBuffer::~ElementBuffer() {
        glDeleteBuffers(1, &m_element_buffer);
    }

    void ElementBuffer::bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer);
    }

    void ElementBuffer::unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void ElementBuffer::upload_data(const void* indices, std::size_t size, buffer::Usage usage) const {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, long(size), indices, usage_enum(usage));
    }
}
