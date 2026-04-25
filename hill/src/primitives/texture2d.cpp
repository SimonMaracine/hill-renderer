#include "hill/primitives/texture2d.hpp"

#include <utility>

#include <glad/gl.h>
#include <glm/gtc/type_ptr.hpp>

namespace hill::texture2d {
    static constexpr int MIPMAPPING_LEVELS = 4;

    static constexpr int wrap_enum(Wrap wrap) {
        switch (wrap) {
            case Wrap::Repeat:
                return GL_REPEAT;
            case Wrap::MirroredRepeat:
                return GL_MIRRORED_REPEAT;
            case Wrap::ClampToEdge:
                return GL_CLAMP_TO_EDGE;
            case Wrap::ClampToBorder:
                return GL_CLAMP_TO_BORDER;
        }

        std::unreachable();
    }

    static constexpr int filter_enum(Filter filter) {
        switch (filter) {
            case Filter::Nearest:
                return GL_NEAREST;
            case Filter::Linear:
                return GL_LINEAR;
        }

        std::unreachable();
    }

    Texture2D::Texture2D() {
        glGenTextures(1, &m_texture);
    }

    Texture2D::~Texture2D() {
        glDeleteTextures(1, &m_texture);
    }

    void Texture2D::bind(unsigned int unit) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, m_texture);
    }

    void Texture2D::unbind(unsigned int unit) const {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture2D::configure(const Specification& specification) const {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_enum(specification.wrap_s));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_enum(specification.wrap_t));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_enum(specification.min_filter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_enum(specification.mag_filter));

        if (specification.wrap_s == Wrap::ClampToBorder || specification.wrap_t == Wrap::ClampToBorder) {
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(specification.border_color));
        }
    }

    void Texture2D::upload_data(Format format, int width, int height, const void* data, bool mipmapping) const {
        switch (format) {
            case Format::Rgba8:
                glTexStorage2D(GL_TEXTURE_2D, mipmapping ? MIPMAPPING_LEVELS : 1, GL_RGBA8, width, height);
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
                break;
        }

        if (mipmapping) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }
    }
}
