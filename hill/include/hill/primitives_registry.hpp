#pragma once

#include <unordered_set>
#include <unordered_map>

namespace hill::primitives_registry {
    enum class Primitive {
        VertexBuffer,
        ElementBuffer,
        VertexArray,
        Shader,
        Program,
        Texture2D
    };

    class Registry {
    public:
        static void initialize();
        static void uninitialize();
        static Registry& get();

        using Container = std::unordered_set<unsigned int>;

        const Container& primitives(Primitive primitive);

        void add_primitive(Primitive primitive, unsigned int id);
        void remove_primitive(Primitive primitive, unsigned int id);
    private:
        std::unordered_map<Primitive, Container> m_primitives;
    };
}
