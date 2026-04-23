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

    bool debug_context() {
        int context_flags {};
        glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);

        return context_flags & GL_CONTEXT_FLAG_DEBUG_BIT;
    }
}
