#pragma once

#include <glm/glm.hpp>

namespace hill::texture2d {
    enum class Wrap {
        Repeat,
        MirroredRepeat,
        ClampToEdge,
        ClampToBorder
    };

    enum class Filter {
        Nearest,
        Linear
    };

    struct Specification {
        Wrap wrap_s = Wrap::Repeat;
        Wrap wrap_t = Wrap::Repeat;
        Filter min_filter = Filter::Nearest;
        Filter mag_filter = Filter::Nearest;
        glm::vec4 border_color {0.0f, 0.0f, 0.0f, 1.0f};
    };

    enum class Format {
        Rgba8
    };

    class Texture2D {
    public:
        Texture2D();
        ~Texture2D();

        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;
        Texture2D(Texture2D&&) = delete;
        Texture2D& operator=(Texture2D&&) = delete;

        unsigned int id() const { return m_texture; }

        void bind(unsigned int unit) const;
        void unbind(unsigned int unit) const;
        void configure(const Specification& specification = {}) const;
        void upload_data(Format format, int width, int height, const void* data, bool mipmapping = false) const;
    private:
        unsigned int m_texture {};
    };
}
