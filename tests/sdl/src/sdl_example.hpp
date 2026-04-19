#pragma once

#include <hill/renderer.hpp>

struct SDL_Window;

class SdlExample {
public:
    SdlExample();
    ~SdlExample();

    SdlExample(const SdlExample&) = delete;
    SdlExample& operator=(const SdlExample&) = delete;
    SdlExample(SdlExample&&) = delete;
    SdlExample& operator=(SdlExample&&) = delete;

    void run();
private:
    hill::renderer::Renderer m_renderer;

    SDL_Window* m_window {};
    void* m_context {};
    bool m_running = true;
};
