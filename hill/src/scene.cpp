#include "hill/scene.hpp"

#include "hill/renderer.hpp"

namespace hill::scene {
    void Node::add(const std::string& name, std::shared_ptr<Node> child) {
        m_children[name] = std::move(child);
    }

    void RootNode::process(renderer::Renderer& renderer) {
        renderer.process_node(this);
    }

    void ModelNode::process(renderer::Renderer& renderer) {
        renderer.process_node(this);
    }
}
