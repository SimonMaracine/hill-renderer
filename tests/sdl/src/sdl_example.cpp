#include "sdl_example.hpp"

#include <stdexcept>
#include <format>

#include <SDL3/SDL.h>

SdlExample::SdlExample() {
    if (!SDL_InitSubSystem(SDL_INIT_VIDEO)) {
        throw std::runtime_error(std::format("SDL_InitSubSystem: %s", SDL_GetError()));
    }

    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4)) {
        throw std::runtime_error(std::format("SDL_GL_SetAttribute: %s", SDL_GetError()));
    }

    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3)) {
        throw std::runtime_error(std::format("SDL_GL_SetAttribute: %s", SDL_GetError()));
    }

    if (!SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE)) {
        throw std::runtime_error(std::format("SDL_GL_SetAttribute: %s", SDL_GetError()));
    }

    if (!SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1)) {
        throw std::runtime_error(std::format("SDL_GL_SetAttribute: %s", SDL_GetError()));
    }

    if (!SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24)) {
        throw std::runtime_error(std::format("SDL_GL_SetAttribute: %s", SDL_GetError()));
    }

    if (!SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1)) {
        throw std::runtime_error(std::format("SDL_GL_SetAttribute: %s", SDL_GetError()));
    }

    static constexpr unsigned int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE;

    m_window = SDL_CreateWindow("SDL Example", 1280, 720, flags);

    if (!m_window) {
        throw std::runtime_error(std::format("SDL_CreateWindow: %s", SDL_GetError()));
    }

    m_context = SDL_GL_CreateContext(m_window);

    if (!m_context) {
        throw std::runtime_error(std::format("SDL_GL_CreateContext: %s", SDL_GetError()));
    }

    m_hill.initialize_graphics_api(SDL_GL_GetProcAddress);

    if (!SDL_GL_SetSwapInterval(1)) {

    }

    if (!SDL_SetWindowMinimumSize(m_window, 640, 360)) {

    }
}

SdlExample::~SdlExample() {
    SDL_GL_DestroyContext(static_cast<SDL_GLContext>(m_context));
    SDL_DestroyWindow(m_window);
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

void SdlExample::run() {
    if (!SDL_ShowWindow(m_window)) {
        throw std::runtime_error(std::format("SDL_ShowWindow: %s", SDL_GetError()));
    }

    m_hill.initialize_renderer();

    while (m_running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    m_running = false;
                    break;
            }
        }

        m_hill.render();

        if (!SDL_GL_SwapWindow(m_window)) {
            throw std::runtime_error(std::format("SDL_GL_SwapWindow: %s", SDL_GetError()));
        }
    }
}
