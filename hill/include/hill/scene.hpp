#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "hill/model.hpp"
#include "hill/renderer_common.hpp"

namespace hill::renderer {
    class Renderer;
}

namespace hill::scene {
    class Node {
    public:
        Node() = default;
        virtual ~Node() = default;

        Node(const Node&) = delete;
        Node& operator=(const Node&) = delete;
        Node(Node&&) = delete;
        Node& operator=(Node&&) = delete;

        virtual void process(renderer::Renderer& renderer) = 0;

        void add(const std::string& name, std::shared_ptr<Node> child);
    protected:
        std::unordered_map<std::string, std::shared_ptr<Node>> m_children;

        friend class renderer::Renderer;
    };

    class RootNode : public Node {
    public:
        void process(renderer::Renderer& renderer) override;
    private:

    };

    class ModelNode : public Node {
    public:
        explicit ModelNode(model::Model model)
            : m_model(std::move(model)) {
            m_objects.reserve(m_model.meshes().size());
        }

        void process(renderer::Renderer& renderer) override;

        glm::mat4 transformation = glm::identity<glm::mat4>();
    private:
        model::Model m_model;
        std::vector<renderer_common::Object> m_objects;
        bool m_configured {};

        friend class renderer::Renderer;
    };
}
