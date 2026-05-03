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

        renderer_command::enable_depth_test();

        m_root_node = std::make_shared<scene::RootNode>();
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
        renderer_command::clear(renderer_command::Buffers::CD);

        m_editor_camera.update_projection_view();

        for (const auto& wprogram : m_programs | std::views::values) {
            if (const auto program = wprogram.lock(); program) {
                program->use();
                program->upload_uniform_float16("u_projection_view", m_editor_camera.projection_view());
                program->unuse();
            }
        }

        render_begin();
        render_traverse_tree(m_root_node.get());
        render_end();

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
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), node->position);
            transform = glm::rotate(transform, glm::radians(node->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
            transform = glm::rotate(transform, glm::radians(node->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
            transform = glm::rotate(transform, glm::radians(node->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
            transform = glm::scale(transform, node->scale);

            object.transform = transform;
        }

        m_objects.append_range(node->m_objects);
    }

    void Renderer::render_begin() {

    }

    void Renderer::render_traverse_tree(scene::Node* tree) {
        for (const auto& node : tree->m_children | std::views::values) {
            node->process(*this);
            render_traverse_tree(node.get());
        }
    }

    void Renderer::render_end() {
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

    void Renderer::process_node(scene::DirectionalLightNode* node) {
        m_directional_light = node->directional_light;
    }

    void Renderer::draw_object(const renderer_common::Object& object) const {
        object.material->m_program->use();
        object.material->use();
        object.vertex_array->bind();

        object.material->m_program->upload_uniform_float16("u_transform", object.transform);
        object.material->m_program->upload_uniform_float3("u_directional_light.direction", m_directional_light.direction);
        object.material->m_program->upload_uniform_float3("u_directional_light.color", m_directional_light.color);

        renderer_command::draw_elements_triangles(object.elements_count, object.elements_offset);

        object.vertex_array->unbind();
        object.material->m_program->unuse();
    }

    void Renderer::configure(scene::ModelNode* node) {
        for (const mesh::Mesh& mesh : node->m_model.meshes()) {
            renderer_common::Object& object = node->m_objects.emplace_back();
            object.elements_count = int(mesh.indices.size());
            object.vertex_array = create_vertex_array(mesh);
            object.material = create_material(mesh, node->m_model);
        }
    }

    std::shared_ptr<vertex_array::VertexArray> Renderer::create_vertex_array(const mesh::Mesh& mesh) const {
        const auto vertices_size = mesh.vertices.size() * (2 * sizeof(glm::vec3));  // TODO
        const auto vertices = std::make_unique<unsigned char[]>(vertices_size);

        for (std::size_t i {}; const mesh::Vertex& vertex : mesh.vertices) {
            std::memcpy(vertices.get() + i, glm::value_ptr(vertex.position), sizeof(vertex.position));
            i += sizeof(vertex.position);
            std::memcpy(vertices.get() + i, glm::value_ptr(vertex.normal), sizeof(vertex.normal));
            i += sizeof(vertex.normal);
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
        layout.attribute(vertex_array::Attribute(0, 3, vertex_array::Type::Float, false, 2 * sizeof(glm::vec3), 0));  // TODO
        layout.attribute(vertex_array::Attribute(1, 3, vertex_array::Type::Float, false, 2 * sizeof(glm::vec3), sizeof(glm::vec3)));

        const auto vertex_array = std::make_shared<vertex_array::VertexArray>();
        vertex_array->bind();
        vertex_array->configure(vertex_buffer, layout);
        vertex_array->configure_and_unbind(element_buffer);

        return vertex_array;
    }

    std::shared_ptr<material::Material> Renderer::create_material(const mesh::Mesh& mesh, const model::Model& model) {
        const ShaderSet shader_set = choose_shader_set(mesh, model);

        if (const auto iter = m_programs.find(shader_set); iter != m_programs.end()) {
            if (const auto program = iter->second.lock(); program) {
                switch (shader_set) {
                    case ShaderSet::Basic: {
                        const auto material = std::make_shared<material::MaterialBasic>(program);
                        material->color = model.materials().at(mesh.material_index).color_diffuse;

                        return material;
                    }
                }
            }
        }

        const auto material = std::make_shared<material::MaterialBasic>(create_program(shader_set));
        material->color = model.materials().at(mesh.material_index).color_diffuse;

        return material;
    }

    std::shared_ptr<shader::Program> Renderer::create_program(ShaderSet shader_set) {
        const char* vertex_shader_source {};
        const char* fragment_shader_source {};

        switch (shader_set) {
            case ShaderSet::Basic:
                vertex_shader_source = vertex_shader_basic();
                fragment_shader_source = fragment_shader_basic();
                break;
        }

        const auto vertex_shader = std::make_shared<shader::Shader>(shader::ShaderType::Vertex);
        vertex_shader->compile(vertex_shader_source);

        const auto fragment_shader = std::make_shared<shader::Shader>(shader::ShaderType::Fragment);
        fragment_shader->compile(fragment_shader_source);

        const auto program = std::make_shared<shader::Program>();
        program->attach_shader(vertex_shader);
        program->attach_shader(fragment_shader);
        program->link();

        m_programs[shader_set] = program;

        return program;
    }

    ShaderSet Renderer::choose_shader_set(const mesh::Mesh& mesh, const model::Model& model) {
        return ShaderSet::Basic;
    }

    const char* Renderer::vertex_shader_basic() {
        return
R"(
    #version 430 core

    layout(location = 0) in vec3 a_position;
    layout(location = 1) in vec3 a_normal;

    out vec3 v_fragment_normal;
    out vec3 v_fragment_position;

    uniform mat4 u_projection_view;
    uniform mat4 u_transform;

    void main() {
        v_fragment_normal = mat3(transpose(inverse(u_transform))) * a_normal;
        v_fragment_position = vec3(u_transform * vec4(a_position, 1.0));
        gl_Position = u_projection_view * u_transform * vec4(a_position, 1.0);
    }
)";
    }

    const char* Renderer::fragment_shader_basic() {
        return
R"(
    #version 430 core

    in vec3 v_fragment_normal;
    in vec3 v_fragment_position;

    layout(location = 0) out vec4 o_color;

    struct MaterialBasic {
        vec3 color;
    };

    uniform MaterialBasic u_material;

    struct DirectionalLight {
        vec3 color;
        vec3 direction;
    };

    uniform DirectionalLight u_directional_light;

    vec3 phong(DirectionalLight directional_light, vec3 object_color, vec3 fragment_normal, vec3 fragment_position) {
        fragment_normal = normalize(fragment_normal);
        const vec3 light_direction = normalize(-directional_light.direction);

        const float ambient_strength = 0.1;
        const vec3 ambient_light = ambient_strength * directional_light.color;

        const float diffuse_strength = max(dot(fragment_normal, light_direction), 0.0);
        const vec3 diffuse_light = diffuse_strength * directional_light.color;

        return (ambient_light + diffuse_light) * object_color;
    }

    void main() {
        const vec3 color = phong(u_directional_light, u_material.color, v_fragment_normal, v_fragment_position);
        o_color = vec4(color, 1.0);
    }
)";
    }
}
