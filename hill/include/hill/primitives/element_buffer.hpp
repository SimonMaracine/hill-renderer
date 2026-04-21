#pragma once

#include <cstddef>

#include "buffer.hpp"

namespace hill::element_buffer {
    class ElementBuffer {
    public:
        ElementBuffer();
        ~ElementBuffer();

        ElementBuffer(const ElementBuffer&) = delete;
        ElementBuffer& operator=(const ElementBuffer&) = delete;
        ElementBuffer(ElementBuffer&&) = delete;
        ElementBuffer& operator=(ElementBuffer&&) = delete;

        unsigned int id() const { return m_element_buffer; }

        void bind() const;
        void unbind() const;
        void upload_data(const void* indices, std::size_t size, buffer::Usage usage = buffer::Usage::StaticDraw) const;
    private:
        unsigned int m_element_buffer {};
    };
}
