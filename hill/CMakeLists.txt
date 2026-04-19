cmake_minimum_required(VERSION 3.20)

include(cmake/glad.cmake)

add_library(hill STATIC)

target_sources(
    hill
    PRIVATE
        "src/glad.cpp"
        "src/graphics_api.cpp"
        "src/renderer.cpp"
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            "include"
        FILES
            "include/hill/configuration.hpp"
            "include/hill/error.hpp"
            "include/hill/graphics_api.hpp"
            "include/hill/renderer.hpp"
)

target_link_libraries(hill PRIVATE glad)

set_target_cpp_mode(hill)
set_target_warnings(hill)
set_target_platform_macros(hill)

if(HILL_DISTRIBUTION)
    target_compile_definitions(hill PRIVATE "NDEBUG")
endif()
