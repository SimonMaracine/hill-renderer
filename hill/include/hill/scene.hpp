#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <format>

#include <glm/glm.hpp>

#include "hill/model.hpp"
#include "hill/light.hpp"
#include "hill/renderer_common.hpp"

namespace hill::renderer {
    struct TraversalCtx;
    class Renderer;
}

namespace hill::editor {
    class Editor;
}

namespace hill::scene {
    class Node : public std::enable_shared_from_this<Node> {
    public:
        Node() = default;
        explicit Node(std::string name)
            : m_name(std::move(name)) {}

        virtual ~Node() = default;

        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
        Node(Node&&) = delete;
        Node& operator=(Node&&) = delete;

        virtual void renderer_process(renderer::Renderer& renderer, renderer::TraversalCtx& ctx) = 0;
        virtual void editor_process(editor::Editor& editor) = 0;

        const std::string& name() const { return m_name; }

        void add(std::shared_ptr<Node> child);
    protected:
        std::string m_name;
        std::unordered_map<std::string, std::shared_ptr<Node>> m_children;

        friend class renderer::Renderer;
        friend class editor::Editor;
    };

    class RootNode : public Node {
    public:
        void renderer_process(renderer::Renderer& renderer, renderer::TraversalCtx& ctx) override;
        void editor_process(editor::Editor& editor) override;
    private:
        friend class renderer::Renderer;
    };

    class MeshNode : public Node {
    public:
        explicit MeshNode(std::string name)
            : Node(std::move(name)) {}

        void renderer_process(renderer::Renderer& renderer, renderer::TraversalCtx& ctx) override;
        void editor_process(editor::Editor& editor) override;
    private:
        renderer_common::Object m_object;

        friend class renderer::Renderer;
        friend class editor::Editor;
    };

    class ModelNode : public Node {
    public:
        ModelNode(std::string name, std::shared_ptr<model::Model> model)
            : Node(std::move(name)), m_model(std::move(model)) {
            for (std::size_t i {}; i < m_model->meshes().size(); i++) {
                add(std::make_shared<MeshNode>(std::format("{}{}", m_name, i)));
            }
        }

        void renderer_process(renderer::Renderer& renderer, renderer::TraversalCtx& ctx) override;
        void editor_process(editor::Editor& editor) override;

        glm::vec3 position {};
        glm::vec3 rotation {};
        glm::vec3 scale {1.0f};
    private:
        std::shared_ptr<model::Model> m_model;
        bool m_configured {};

        friend class renderer::Renderer;
    };

    class DirectionalLightNode : public Node {
    public:
        explicit DirectionalLightNode(std::string name, const light::DirectionalLight& directional_light = {})
            : Node(std::move(name)), directional_light(directional_light) {}

        void renderer_process(renderer::Renderer& renderer, renderer::TraversalCtx& ctx) override;
        void editor_process(editor::Editor& editor) override;

        light::DirectionalLight directional_light;
    private:
        friend class renderer::Renderer;
    };
}
