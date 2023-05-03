#ifndef MODEL_HPP
#define MODEL_HPP

#include <vector>

#include <assimp/material.h>
#include <assimp/scene.h>

#include "Mesh.hpp"
#include "Texture.hpp"

namespace StarBear {

class Model
{
  public:
    void LoadFromFile(const std::string& aFile);

    void Draw(const Shader& aShader,
              GLenum aMode = GL_TRIANGLES) const;
    void DrawInstanced(const Shader& aShader,
                       int aNumInstances,
                       GLenum aMode = GL_TRIANGLES) const;

  private:
    void ProcessNode(aiNode& aNode,
                     const aiScene& aScene,
                     const std::string& aWorkingDirectory);
    void ProcessMesh(aiMesh& aMesh,
                     unsigned int aMeshIndex,
                     const aiScene& aScene,
                     const std::string& aWorkingDirectory);
    void ProcessMaterialTextures(aiMaterial& aMaterial,
                                 aiTextureType aTextureType,
                                 const std::string& aWorkingDirectory);

    std::vector<Mesh> mMeshes;
    std::vector<Texture> mTextures;
};

} // namespace StarBear

#endif
