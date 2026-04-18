#pragma once

#include "hill/configuration.hpp"

namespace hill {
    using ApiProcedure = void(*)();
    using LoadFunction = ApiProcedure(*)(const char*);

    class HillRenderer {
    public:
        HillRenderer() = default;
        explicit HillRenderer(const Configuration& configuration);
        ~HillRenderer();

        HillRenderer(const HillRenderer&) = delete;
        HillRenderer& operator=(const HillRenderer&) = delete;
        HillRenderer(HillRenderer&&) = delete;
        HillRenderer& operator=(HillRenderer&&) = delete;

        void initialize_graphics_api(LoadFunction load_function);
        void initialize_renderer();
        void render();
    private:
        Configuration m_configuration;
    };
}
