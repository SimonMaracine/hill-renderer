#include "hill/renderer.hpp"

#include <glad/gl.h>  // FIXME remove

namespace hill::renderer {
    Renderer::Renderer(const configuration::Configuration& configuration)
        : m_configuration(configuration) {}

    Renderer::~Renderer() {

    }

    void Renderer::initialize() {
        glClearColor(0.4f, 0.1f, 0.6f, 1.0f);
    }

    void Renderer::uninitialize() {

    }

    void Renderer::render() {
        glClear(GL_COLOR_BUFFER_BIT);
    }
}
