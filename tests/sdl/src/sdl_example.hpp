#pragma once

#include <hill/hill.hpp>

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
    hill::HillRenderer m_hill;

    SDL_Window* m_window {};
    void* m_context {};
    bool m_running = true;
};
