#include "hill/editor.hpp"

#include <imgui.h>

#include "hill/renderer.hpp"

namespace hill::editor {
    void Editor::update(renderer::Renderer& renderer) {
        ImGui::ShowDemoWindow();

        ImGui::Begin("Test");
        ImGui::ColorEdit3("Background Color", renderer.m_background_color);
        ImGui::End();
    }
}
