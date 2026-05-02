#include "hill/editor.hpp"

#include <ranges>

#include <imgui.h>

#include "hill/renderer.hpp"
#include "hill/primitives_registry.hpp"
#include "hill/scene.hpp"

namespace hill::editor {
    void Editor::initialize() {
        auto& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    }

    void Editor::update(renderer::Renderer& renderer) {
        ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

        ImGui::ShowDemoWindow();

        ImGui::Begin("Test");
        ImGui::ColorEdit3("Background Color", renderer.m_background_color);
        ImGui::End();

        performance(renderer);
        primitives_registry(renderer);
        scene_hierarchy(renderer);
        node_properties(renderer);
    }

    void Editor::update_camera(renderer::Renderer& renderer) {
        static constexpr float SHIFT_SPEED = 5.0f;
        static constexpr float LOOK_SPEED = 7.0f;

        if (!ImGui::GetIO().WantCaptureMouse) {
            if (ImGui::IsMouseDragging(ImGuiMouseButton_Right)) {
                const ImVec2 delta = ImGui::GetIO().MouseDelta;
                m_camera.yaw += -delta.x * LOOK_SPEED * ImGui::GetIO().DeltaTime;
                m_camera.pitch += delta.y * LOOK_SPEED * ImGui::GetIO().DeltaTime;
            }
        }

        m_camera.pitch = glm::min(m_camera.pitch, 89.0f);
        m_camera.pitch = glm::max(m_camera.pitch, -89.0f);

        if (!ImGui::GetIO().WantCaptureKeyboard) {
            if (ImGui::IsKeyDown(ImGuiKey_W)) {
                m_camera.position += m_camera.front * SHIFT_SPEED * ImGui::GetIO().DeltaTime;
            } else if (ImGui::IsKeyDown(ImGuiKey_S)) {
                m_camera.position -= m_camera.front * SHIFT_SPEED * ImGui::GetIO().DeltaTime;
            } else if (ImGui::IsKeyDown(ImGuiKey_A)) {
                m_camera.position -= glm::normalize(glm::cross(m_camera.front, m_camera.up)) * SHIFT_SPEED * ImGui::GetIO().DeltaTime;
            } else if (ImGui::IsKeyDown(ImGuiKey_D)) {
                m_camera.position += glm::normalize(glm::cross(m_camera.front, m_camera.up)) * SHIFT_SPEED * ImGui::GetIO().DeltaTime;
            } else if (ImGui::IsKeyDown(ImGuiKey_E)) {
                m_camera.position += m_camera.up * SHIFT_SPEED * ImGui::GetIO().DeltaTime;
            } else if (ImGui::IsKeyDown(ImGuiKey_Q)) {
                m_camera.position -= m_camera.up * SHIFT_SPEED * ImGui::GetIO().DeltaTime;
            }
        }

        glm::vec3 direction {};
        direction.x = glm::cos(glm::radians(m_camera.yaw)) * glm::cos(glm::radians(m_camera.pitch));
        direction.y = glm::sin(glm::radians(m_camera.pitch));
        direction.z = glm::sin(glm::radians(m_camera.yaw)) * glm::cos(glm::radians(m_camera.pitch));
        m_camera.front = glm::normalize(direction);

        renderer.m_editor_camera.position_orientation(m_camera.position, m_camera.position + m_camera.front, m_camera.up);
    }

    void Editor::performance(renderer::Renderer& renderer) {
        if (ImGui::Begin("Performance")) {
            ImGui::Text("Frame time: %.02f", renderer.m_frame_time.count() * 1000.0);
        }

        ImGui::End();
    }

    void Editor::primitives_registry(renderer::Renderer&) {
        if (ImGui::Begin("Primitives Registry")) {
            primitives_object("Vertex Buffers", primitives_registry::Registry::get().vertex_buffers());
            primitives_object("Element Buffers", primitives_registry::Registry::get().element_buffers());
            primitives_object("Vertex Arrays", primitives_registry::Registry::get().vertex_arrays());
            primitives_object("Shaders", primitives_registry::Registry::get().shaders());
            primitives_object("Programs", primitives_registry::Registry::get().programs());
            primitives_object("Textures 2D", primitives_registry::Registry::get().textures2d());
        }

        ImGui::End();
    }

    void Editor::primitives_object(const char* label, const auto& objects) {
        if (ImGui::TreeNode(label)) {
            for (const auto [i, object] : objects | std::views::enumerate) {
                ImGui::Text("%ld. %u", i + 1, object);
            }

            ImGui::TreePop();
        }
    }

    void Editor::scene_hierarchy(renderer::Renderer& renderer) {
        if (ImGui::Begin("Scene Hierarchy")) {
            scene_hierarchy_tree(renderer.root_node(), "");
        }

        ImGui::End();
    }

    void Editor::scene_hierarchy_tree(scene::Node* tree, std::string path) {
        using namespace std::string_literals;

        path += tree->name().data() + "/"s;

        if (ImGui::TreeNode(path.c_str(), "%s", tree->name().empty() ? "/" : tree->name().data())) {
            for (const auto& node : tree->m_children | std::views::values) {
                scene_hierarchy_tree(node.get(), path);
            }

            ImGui::TreePop();
        }
    }

    void Editor::node_properties(renderer::Renderer& renderer) {
        if (ImGui::Begin("Node Properties")) {

        }

        ImGui::End();
    }
}
