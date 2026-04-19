#include "hill/graphics_api.hpp"

#include <glad/gl.h>

#include "hill/error.hpp"

namespace hill::graphics_api {
    void initialize(LoadFunction load_function) {
        if (!gladLoadGL(load_function)) {
            throw error::Error("Could not load the OpenGL functions");
        }
    }

    const char* version() {
        return reinterpret_cast<const char*>(glGetString(GL_VERSION));
    }
}
