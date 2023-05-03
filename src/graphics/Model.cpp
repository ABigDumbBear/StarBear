#include "Model.hpp"

#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace StarBear {

/******************************************************************************/
void Model::LoadFromFile(const std::string& aFile)
{
  mMeshes.clear();

  Assimp::Importer importer;
  auto scene = importer.ReadFile(aFile,
                                 aiProcess_Triangulate | aiProcess_FlipUVs);

  auto workingDirectory = aFile.substr(0, aFile.find_last_of("/\\") + 1);
  ProcessNode(*scene->mRootNode, *scene, workingDirectory);
}

/******************************************************************************/
void Model::Draw(const Shader& aShader, GLenum aMode) const
{
  for(size_t i = 0; i < mTextures.size(); ++i)
  {
    glActiveTexture(GL_TEXTURE0 + i);
    glBindTexture(GL_TEXTURE_2D, mTextures[i].GetID());
  }

  for(const auto& mesh : mMeshes)
  {
    mesh.Draw(aShader, aMode);
  }
}

/******************************************************************************/
void Model::DrawInstanced(const Shader& aShader, int aNumInstances, GLenum aMode) const
{
  for(const auto& mesh : mMeshes)
  {
    mesh.DrawInstanced(aShader, aNumInstances, aMode);
  }
}

/******************************************************************************/
void Model::ProcessNode(aiNode& aNode,
                        const aiScene& aScene,
                        const std::string& aWorkingDirectory)
{
  // Process each mesh in this node.
  for(int i = 0; i < aNode.mNumMeshes; ++i)
  {
    auto mesh = aScene.mMeshes[aNode.mMeshes[i]];
    ProcessMesh(*mesh, i, aScene, aWorkingDirectory);
  }

  // Process each of this node's children.
  for(int i = 0; i < aNode.mNumChildren; ++i)
  {
    ProcessNode(*aNode.mChildren[i], aScene, aWorkingDirectory);
  }
}

/******************************************************************************/
void Model::ProcessMesh(aiMesh& aMesh,
                        unsigned int aMeshIndex,
                        const aiScene& aScene,
                        const std::string& aWorkingDirectory)
{
  Mesh mesh;

  // Retrieve the vertex data.
  for(int i = 0; i < aMesh.mNumVertices; ++i)
  {
    MeshVertex vertex;

    // Default the vertex color to white.
    vertex.mColor = Vec3(1.0, 1.0, 1.0);

    // Retrieve the position data.
    vertex.mPosition = Vec3(aMesh.mVertices[i].x,
                            aMesh.mVertices[i].y,
                            aMesh.mVertices[i].z);

    // Retrieve the texture coordinates.
    auto texCoords = aMesh.mTextureCoords[0];
    if(texCoords != nullptr)
    {
      vertex.mTexCoords[0] = texCoords[i].x;
      vertex.mTexCoords[1] = texCoords[i].y;
    }

    mesh.mVertices.emplace_back(vertex);
  }

  // Retrieve the index data.
  for(int i = 0; i < aMesh.mNumFaces; ++i)
  {
    auto face = aMesh.mFaces[i];
    for(int j = 0; j < face.mNumIndices; ++j)
    {
      mesh.mIndices.emplace_back(face.mIndices[j]);
    }
  }

  // Retrieve the material data.
  if(aMesh.mMaterialIndex >= 0)
  {
    auto& material = *aScene.mMaterials[aMesh.mMaterialIndex];
    ProcessMaterialTextures(material, aiTextureType_DIFFUSE, aWorkingDirectory);
  }

  mesh.UpdateVertices();
  mesh.UpdateIndices();

  mMeshes.emplace_back(std::move(mesh));
}

/******************************************************************************/
void Model::ProcessMaterialTextures(aiMaterial& aMaterial,
                                    aiTextureType aTextureType,
                                    const std::string& aWorkingDirectory)
{
  std::stringstream path;
  for(size_t i = 0; i < aMaterial.GetTextureCount(aTextureType); ++i)
  {
    aiString str;
    aMaterial.GetTexture(aTextureType, i, &str);

    path << aWorkingDirectory << str.C_Str();

    Texture texture;
    texture.LoadFromFile(path.str());
    mTextures.emplace_back(std::move(texture));

    path.str("");
  }
}

} // namespace StarBear
