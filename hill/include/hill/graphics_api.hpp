#pragma once

namespace hill::graphics_api {
    using ApiProcedure = void(*)();
    using LoadFunction = ApiProcedure(*)(const char*);

    void initialize(LoadFunction load_function);
    const char* version();
    bool debug_context();
}
