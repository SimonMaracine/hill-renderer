#include "hill/model.hpp"

#include <format>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace hill::model {
    Model::Model(const utility::Buffer& buffer) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFileFromMemory(buffer.data(), buffer.size(), aiProcess_Triangulate);

        if (!scene) {
            throw ModelError(std::format("Could not load model: {}", importer.GetErrorString()));
        }
    }

    void Model::process_node(aiNode* node, const aiScene* scene) {

    }

    mesh::Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene) {

    }
}
