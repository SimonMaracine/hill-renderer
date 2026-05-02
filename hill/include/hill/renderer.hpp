#pragma once

#include <memory>
#include <chrono>
#include <vector>
#include <unordered_map>

#include "hill/configuration.hpp"
#include "hill/imgui.hpp"
#include "hill/camera.hpp"
#include "hill/scene.hpp"
#include "hill/light.hpp"
#include "hill/material.hpp"
#include "hill/renderer_common.hpp"

namespace hill::editor {
    class Editor;
}

namespace hill::renderer {
    enum class ShaderSet {
        Basic
    };

    class Renderer {
    public:
        explicit Renderer(imgui::ImGui& imgui);
        Renderer(imgui::ImGui& imgui, const configuration::Configuration& configuration);

        void initialize();
        void uninitialize();
        void render();
        void window_resize(int width, int height);

        scene::RootNode* root_node() const { return m_root_node.get(); }
    private:
        void imgui_initialize() const;
        void imgui_uninitialize() const;
        void imgui_render() const;

        void submit(scene::ModelNode* node);

        void render_begin();
        void render_traverse_tree(scene::Node* tree);
        void render_end();
        void process_node(scene::RootNode* node);
        void process_node(scene::ModelNode* node);
        void process_node(scene::DirectionalLightNode* node);

        void draw_object(const renderer_common::Object& object) const;

        void configure(scene::ModelNode* node);

        std::shared_ptr<vertex_array::VertexArray> create_vertex_array(const mesh::Mesh& mesh) const;
        std::shared_ptr<material::Material> create_material(const mesh::Mesh& mesh, const model::Model& model);
        std::shared_ptr<shader::Program> create_program(ShaderSet shader_set);

        static ShaderSet choose_shader_set(const mesh::Mesh& mesh, const model::Model& model);
        static const char* vertex_shader_basic();
        static const char* fragment_shader_basic();

        imgui::ImGui* m_imgui {};
        configuration::Configuration m_configuration;

        int m_window_width {};
        int m_window_height {};

        float m_background_color[3] { 0.4f, 0.1f, 0.6f };

        camera::Camera m_editor_camera;
        light::DirectionalLight m_directional_light;

        std::unique_ptr<scene::RootNode> m_root_node;

        std::vector<renderer_common::Object> m_objects;
        std::unordered_map<ShaderSet, std::weak_ptr<shader::Program>> m_programs;

        std::chrono::high_resolution_clock::time_point m_last_time {};
        std::chrono::duration<double> m_frame_time {};

        friend class editor::Editor;
        friend class scene::RootNode;
        friend class scene::ModelNode;
        friend class scene::DirectionalLightNode;
    };
}
