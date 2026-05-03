#include "hill/primitives_registry.hpp"

#include <memory>

namespace hill::primitives_registry {
    static std::unique_ptr<Registry> g_instance;

    void Registry::initialize() {
        g_instance = std::make_unique<Registry>();
    }

    void Registry::uninitialize() {
        g_instance.reset();
    }

    Registry& Registry::get() {
        return *g_instance;
    }

    const Registry::Container& Registry::primitives(Primitive primitive) {
        return m_primitives[primitive];
    }

    void Registry::add_primitive(Primitive primitive, unsigned int id) {
        m_primitives[primitive].insert(id);
    }

    void Registry::remove_primitive(Primitive primitive, unsigned int id) {
        m_primitives[primitive].erase(id);
    }
}
