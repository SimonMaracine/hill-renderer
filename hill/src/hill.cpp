#include "hill/hill.hpp"

#include <glad/gl.h>

#include "hill/error.hpp"

namespace hill {
    HillRenderer::HillRenderer(const Configuration& configuration)
        : m_configuration(configuration) {}

    HillRenderer::~HillRenderer() {

    }

    void HillRenderer::initialize_graphics_api(LoadFunction load_function) {
        if (!gladLoadGL(load_function)) {
            throw Error("Could not load GLAD");
        }
    }

    void HillRenderer::initialize_renderer() {
        glClearColor(0.4f, 0.1f, 0.6f, 1.0f);
    }

    void HillRenderer::render() {
        glClear(GL_COLOR_BUFFER_BIT);
    }
}
