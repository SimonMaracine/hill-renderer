#pragma once

#include <glm/glm.hpp>

namespace hill::light {
    struct DirectionalLight {
        glm::vec3 direction {};
        glm::vec3 color {};
    };
}
