#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace hill::mesh {
    struct Vertex {
        glm::vec3 position {};
        glm::vec3 color {};
        glm::vec3 normal {};
        glm::vec2 texture_coordinate {};
    };

    enum class TextureType {
        Albedo,
        Specular,
        Normal
    };

    struct Texture {
        unsigned int texture {};
        TextureType type {};
    };

    struct Mesh {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
    };
}
