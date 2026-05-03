#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include <glm/glm.hpp>

#include "hill/model.hpp"
#include "hill/light.hpp"
#include "hill/renderer_common.hpp"

namespace hill::renderer {
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

        virtual void process(renderer::Renderer& renderer) = 0;

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
        void process(renderer::Renderer& renderer) override;
    private:
        friend class renderer::Renderer;
    };

    class ModelNode : public Node {
    public:
        ModelNode(std::string name, model::Model model)
            : Node(std::move(name)), m_model(std::move(model)) {
            m_objects.reserve(m_model.meshes().size());
        }

        void process(renderer::Renderer& renderer) override;

        glm::vec3 position {};
        glm::vec3 rotation {};
        glm::vec3 scale {1.0f};
    private:
        model::Model m_model;
        std::vector<renderer_common::Object> m_objects;
        bool m_configured {};

        friend class renderer::Renderer;
    };

    class DirectionalLightNode : public Node {
    public:
        explicit DirectionalLightNode(std::string name, const light::DirectionalLight& directional_light = {})
            : Node(std::move(name)), directional_light(directional_light) {}

        void process(renderer::Renderer& renderer) override;

        light::DirectionalLight directional_light;
    private:
        friend class renderer::Renderer;
    };
}
