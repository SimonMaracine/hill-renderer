#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "hill/primitives/shader.hpp"

namespace hill::renderer {
    class Renderer;
}

namespace hill::material {
    class Material {
    public:
        explicit Material(std::shared_ptr<shader::Program> program)
            : m_program(std::move(program)) {}

        virtual ~Material() = default;

        Material(const Material&) = default;
        Material& operator=(const Material&) = default;
        Material(Material&&) = default;
        Material& operator=(Material&&) = default;

        virtual void use() = 0;
    protected:
        std::shared_ptr<shader::Program> m_program;

        friend class renderer::Renderer;
    };

    struct MaterialBasic : Material {
        using Material::Material;

        void use() override {
            m_program->upload_uniform_float3("u_material.color", color);
        }

        glm::vec3 color {};
    };
}
