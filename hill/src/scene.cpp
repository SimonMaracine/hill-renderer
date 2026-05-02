#include "hill/scene.hpp"

#include "hill/renderer.hpp"

namespace hill::scene {
    void Node::add(std::shared_ptr<Node> child) {
        const auto name = child->name();
        m_children[std::string(name)] = std::move(child);
    }

    void RootNode::process(renderer::Renderer& renderer) {
        renderer.process_node(this);
    }

    void ModelNode::process(renderer::Renderer& renderer) {
        renderer.process_node(this);
    }

    void DirectionalLightNode::process(renderer::Renderer& renderer) {
        renderer.process_node(this);
    }
}
