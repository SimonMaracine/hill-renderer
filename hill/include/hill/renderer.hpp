#pragma once

#include "hill/configuration.hpp"

namespace hill::renderer {
    class Renderer {
    public:
        Renderer() = default;
        explicit Renderer(const configuration::Configuration& configuration);
        ~Renderer();

        Renderer(const Renderer&) = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer(Renderer&&) = delete;
        Renderer& operator=(Renderer&&) = delete;

        void initialize();
        void uninitialize();
        void render();
    private:
        configuration::Configuration m_configuration;
    };
}
