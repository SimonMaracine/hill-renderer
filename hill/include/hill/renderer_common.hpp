#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "hill/primitives/shader.hpp"
#include "hill/primitives/vertex_array.hpp"

namespace hill::renderer_common {
    struct Object {
        glm::mat4 transformation = glm::identity<glm::mat4>();
        int elements_count {};
        int elements_offset {};
        std::shared_ptr<vertex_array::VertexArray> vertex_array;
        std::shared_ptr<shader::Program> program;
    };
}
