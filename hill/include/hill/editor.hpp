#pragma once

#include <string>

#include <glm/glm.hpp>

namespace hill::renderer {
    class Renderer;
}

namespace hill::scene {
    class Node;
}

namespace hill::editor {
    class Editor {
    public:
        void initialize();
        void update(renderer::Renderer& renderer);
        void update_camera(renderer::Renderer& renderer);
    private:
        void performance(renderer::Renderer& renderer);

        void primitives_registry(renderer::Renderer& renderer);
        void primitives_object(const char* label, const auto& objects);

        void scene_hierarchy(renderer::Renderer& renderer);
        void scene_hierarchy_tree(scene::Node* tree, std::string path);

        void node_properties(renderer::Renderer& renderer);

        struct Camera {
            glm::vec3 position {0.0f, 0.0f, 2.0f};
            glm::vec3 front {0.0f, 0.0f, -1.0f};
            glm::vec3 up {0.0f, 1.0f, 0.0f};

            float pitch {};
            float yaw = -90.0f;
        } m_camera;
    };
}
