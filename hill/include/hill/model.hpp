#pragma once

#include <vector>
#include <unordered_map>

#include "hill/mesh.hpp"
#include "hill/utility.hpp"
#include "hill/error.hpp"

struct aiScene;
struct aiNode;
struct aiMesh;

namespace hill::model {
    class Model {
    public:
        explicit Model(const utility::Buffer& buffer);
        explicit Model(const utility::FilePath& file_path);

        const std::vector<mesh::Mesh>& meshes() const { return m_meshes; }
        const std::unordered_map<unsigned int, mesh::Material>& materials() const { return m_materials; }
    private:
        void process_node(const aiNode* node, const aiScene* scene);
        mesh::Mesh process_mesh(const aiMesh* mesh, const aiScene* scene);

        std::vector<mesh::Mesh> m_meshes;
        std::unordered_map<unsigned int, mesh::Material> m_materials;
    };

    struct ModelError : error::Error {
        using Error::Error;
    };
}
