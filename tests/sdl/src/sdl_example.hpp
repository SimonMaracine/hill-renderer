#pragma once

#include <hill/renderer.hpp>
#include <hill/editor.hpp>

struct SDL_Window;

class SdlExample : public hill::imgui::ImGui {
public:
    SdlExample();
    ~SdlExample() override;

    SdlExample(const SdlExample&) = delete;
    SdlExample& operator=(const SdlExample&) = delete;
    SdlExample(SdlExample&&) = delete;
    SdlExample& operator=(SdlExample&&) = delete;

    void initialize() const override;
    void uninitialize() const override;
    void begin() const override;
    void end(ImDrawData* draw_data) const override;
    void update() override;

    void run();
private:
    hill::renderer::Renderer m_renderer;
    hill::editor::Editor m_editor;

    SDL_Window* m_window {};
    void* m_context {};
    bool m_running = true;
};
