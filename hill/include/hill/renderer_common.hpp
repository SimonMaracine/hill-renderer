#pragma once

#include <memory>

#include "hill/primitives/vertex_array.hpp"
#include "hill/material.hpp"

namespace hill::renderer_common {
    struct Object {
        int elements_count {};
        int elements_offset {};
        std::shared_ptr<vertex_array::VertexArray> vertex_array;
        std::shared_ptr<material::Material> material;
    };
}
