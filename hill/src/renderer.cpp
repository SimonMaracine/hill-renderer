#include "hill/renderer.hpp"

#include <ranges>
#include <cstring>

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include "hill/primitives/vertex_buffer.hpp"
#include "hill/primitives/element_buffer.hpp"
#include "hill/graphics_api.hpp"
#include "hill/renderer_command.hpp"
#include "hill/debug.hpp"
#include "hill/primitives_registry.hpp"

namespace hill::renderer {
    Renderer::Renderer(imgui::ImGui& imgui)
        : m_imgui(&imgui) {}

    Renderer::Renderer(imgui::ImGui& imgui, const configuration::Configuration& configuration)
        : m_imgui(&imgui), m_configuration(configuration) {}

    void Renderer::initialize() {
        if (m_configuration.debug_output && graphics_api::debug_context()) {
            debug::initialize(*m_configuration.debug_output);
        }

        imgui_initialize();
        primitives_registry::Registry::initialize();

        m_root_node = std::make_unique<scene::RootNode>();
        m_last_time = std::chrono::high_resolution_clock::now();
    }

    void Renderer::uninitialize() {
        m_root_node.reset();

        primitives_registry::Registry::uninitialize();
        imgui_uninitialize();
    }

    void Renderer::render() {
        const auto current_time = std::chrono::high_resolution_clock::now();
        m_frame_time = current_time - m_last_time;
        m_last_time = current_time;

        m_editor_camera.projection(m_window_width, m_window_height, 45.0f, 0.01f, 100.0f);
        renderer_command::viewport(m_window_width, m_window_height);

        renderer_command::clear_color({ m_background_color[0], m_background_color[1], m_background_color[2], 1.0f });
        renderer_command::clear(renderer_command::Buffers::C);

        m_editor_camera.update_projection_view();

        for (const auto& wprogram : m_programs | std::views::values) {
            if (const auto program = wprogram.lock(); program) {
                program->use();
                program->upload_uniform_float16("u_projection_view", m_editor_camera.projection_view());
                program->unuse();
            }
        }

        begin();
        traverse_tree(m_root_node.get());
        end();

        imgui_render();
    }

    void Renderer::window_resize(int width, int height) {
        m_window_width = width;
        m_window_height = height;
    }

    void Renderer::imgui_initialize() const {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        m_imgui->initialize();
    }

    void Renderer::imgui_uninitialize() const {
        m_imgui->uninitialize();
        ImGui::DestroyContext();
    }

    void Renderer::imgui_render() const {
        m_imgui->begin();
        ImGui::NewFrame();

        m_imgui->update();

        ImGui::EndFrame();
        ImGui::Render();
        m_imgui->end(ImGui::GetDrawData());
    }

    void Renderer::submit(scene::ModelNode* node) {
        if (!node->m_configured) {
            configure(node);
            node->m_configured = true;
        }

        for (renderer_common::Object& object : node->m_objects) {
            object.transformation = node->transformation;
        }

        m_objects.append_range(node->m_objects);
    }

    void Renderer::begin() {

    }

    void Renderer::traverse_tree(scene::Node* tree) {
        for (const auto& node : tree->m_children | std::views::values) {
            node->process(*this);
            traverse_tree(node.get());
        }
    }

    void Renderer::end() {
        for (const renderer_common::Object& object : m_objects) {
            draw_object(object);
        }

        m_objects.clear();
    }

    void Renderer::process_node(scene::RootNode* node) {

    }

    void Renderer::process_node(scene::ModelNode* node) {
        submit(node);
    }

    void Renderer::draw_object(const renderer_common::Object& object) const {
        object.program->use();
        object.vertex_array->bind();

        object.program->upload_uniform_float16("u_transform", object.transformation);
        renderer_command::draw_elements_triangles(object.elements_count, object.elements_offset);

        object.vertex_array->unbind();
        object.program->unuse();
    }

    void Renderer::configure(scene::ModelNode* node) {
        for (const mesh::Mesh& mesh : node->m_model.meshes()) {
            renderer_common::Object& object = node->m_objects.emplace_back();
            object.elements_count = int(mesh.indices.size());
            object.vertex_array = create_vertex_array(mesh);
            object.program = create_program(mesh);
        }
    }

    std::shared_ptr<vertex_array::VertexArray> Renderer::create_vertex_array(const mesh::Mesh& mesh) const {
        const auto vertices_size = mesh.vertices.size() * sizeof(glm::vec3);  // TODO
        const auto vertices = std::make_unique<unsigned char[]>(vertices_size);

        for (std::size_t i {}; const mesh::Vertex& vertex : mesh.vertices) {
            std::memcpy(vertices.get() + i, glm::value_ptr(vertex.position), sizeof(vertex.position));
            i += sizeof(vertex.position);
        }

        const auto vertex_buffer = std::make_shared<vertex_buffer::VertexBuffer>();
        vertex_buffer->bind();
        vertex_buffer->upload_data(vertices.get(), vertices_size);
        vertex_buffer->unbind();

        const auto element_buffer = std::make_shared<element_buffer::ElementBuffer>();
        element_buffer->bind();
        element_buffer->upload_data(mesh.indices.data(), mesh.indices.size() * sizeof(unsigned int));
        element_buffer->unbind();

        vertex_array::Layout layout;
        layout.attribute(vertex_array::Attribute(0, 3, vertex_array::Type::Float, false, sizeof(glm::vec3), 0));  // TODO

        const auto vertex_array = std::make_shared<vertex_array::VertexArray>();
        vertex_array->bind();
        vertex_array->configure(vertex_buffer, layout);
        vertex_array->configure_and_unbind(element_buffer);

        return vertex_array;
    }

    std::shared_ptr<shader::Program> Renderer::create_program(const mesh::Mesh& mesh) {
        if (const auto iter = m_programs.find("basic"); iter != m_programs.end()) {  // TODO
            if (const auto program = iter->second.lock(); program) {
                return program;
            }
        }

        constexpr const char* vertex_shader_source =  // TODO
R"(
    #version 430 core

    layout(location = 0) in vec3 a_position;

    uniform mat4 u_projection_view;
    uniform mat4 u_transform;

    void main() {
        gl_Position = u_projection_view * u_transform * vec4(a_position, 1.0);
    }
)";

        constexpr const char* fragment_shader_source =
R"(
    #version 430 core

    layout(location = 0) out vec4 o_color;

    void main() {
        o_color = vec4(0.5, 0.5, 0.5, 1.0);
    }
)";

        const auto vertex_shader = std::make_shared<shader::Shader>(shader::ShaderType::Vertex);
        vertex_shader->compile(vertex_shader_source);

        const auto fragment_shader = std::make_shared<shader::Shader>(shader::ShaderType::Fragment);
        fragment_shader->compile(fragment_shader_source);

        const auto program = std::make_shared<shader::Program>();
        program->attach_shader(vertex_shader);
        program->attach_shader(fragment_shader);
        program->link();

        m_programs["basic"] = program;

        return program;
    }
}
