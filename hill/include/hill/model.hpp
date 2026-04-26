#pragma once

#include <vector>

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
    private:
        void process_node(aiNode* node, const aiScene* scene);
        mesh::Mesh process_mesh(aiMesh* mesh, const aiScene* scene);

        std::vector<mesh::Mesh> meshes;
    };

    struct ModelError : error::Error {
        using Error::Error;
    };
}
