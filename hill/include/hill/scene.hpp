#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

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
    class Node {
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

    };

    class ModelNode : public Node {
    public:
        ModelNode(std::string name, model::Model model)
            : Node(std::move(name)), m_model(std::move(model)) {
            m_objects.reserve(m_model.meshes().size());
        }

        void process(renderer::Renderer& renderer) override;

        glm::mat4 transform = glm::identity<glm::mat4>();
    private:
        model::Model m_model;
        std::vector<renderer_common::Object> m_objects;
        bool m_configured {};

        friend class renderer::Renderer;
    };

    class DirectionalLightNode : public Node {
    public:
        DirectionalLightNode(std::string name, const light::DirectionalLight& directional_light)
            : Node(std::move(name)), m_directional_light(directional_light) {}

        void process(renderer::Renderer& renderer) override;
    private:
        light::DirectionalLight m_directional_light;

        friend class renderer::Renderer;
    };
}
