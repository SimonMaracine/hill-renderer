add_library(
    glad
    INTERFACE
    "extern/glad/include/glad/gl.h"
    "extern/glad/include/glad/gles2.h"
)

target_include_directories(glad INTERFACE "extern/glad/include")
