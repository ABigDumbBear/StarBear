#include "ResourceMap.hpp"

namespace StarBear {

/******************************************************************************/
ResourceMap::ResourceMap()
{
  // Load individual meshes.
  mMeshMap.emplace(MeshType::eQUAD, Mesh());
  mMeshMap.emplace(MeshType::eCUBE, Mesh());

  mMeshMap[MeshType::eQUAD].InitQuad();
  mMeshMap[MeshType::eCUBE].InitCube();

  glBindVertexArray(mMeshMap[MeshType::eQUAD].GetVertexArrayID());
  glBindBuffer(GL_ARRAY_BUFFER, mMeshMap[MeshType::eQUAD].GetCustomBufferID());
  glEnableVertexAttribArray(8);
  glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glVertexAttribDivisor(8, 1);
  glBindVertexArray(0);

  // Load individual textures.
  mTextureMap.emplace(TextureType::eCROSSHAIR, Texture());
  mTextureMap.emplace(TextureType::ePARTICLE, Texture());

  mTextureMap[TextureType::eCROSSHAIR].LoadFromFile("resources/crosshair.png");
  mTextureMap[TextureType::ePARTICLE].LoadFromFile("resources/particle.png");

  // Load models.
  mModelMap.emplace(ModelType::eBOB, Model());
  mModelMap.emplace(ModelType::eSPITFIRE, Model());

  mModelMap[ModelType::eBOB].LoadFromFile("resources/models/Bob/Bob.obj");
  mModelMap[ModelType::eSPITFIRE].LoadFromFile("resources/models/Spitfire/Spitfire.obj");

  for(const auto& mesh : mModelMap[ModelType::eBOB].GetMeshes())
  {
    glBindVertexArray(mesh.GetVertexArrayID());

    glBindBuffer(GL_ARRAY_BUFFER, mesh.GetCustomBufferID());
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8,
                          1,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (void*)0);

    glVertexAttribDivisor(8, 1);

    glBindVertexArray(0);
  }

  for(const auto& mesh : mModelMap[ModelType::eSPITFIRE].GetMeshes())
  {
    glBindVertexArray(mesh.GetVertexArrayID());

    glBindBuffer(GL_ARRAY_BUFFER, mesh.GetCustomBufferID());
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8,
                          1,
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          (void*)0);

    glVertexAttribDivisor(8, 1);

    glBindVertexArray(0);
  }

  // Load shaders.
  mShaderMap.emplace(ShaderType::eSHIP, Shader());
  mShaderMap.emplace(ShaderType::ePARTICLE, Shader());
  mShaderMap.emplace(ShaderType::eHITBOX, Shader());

  mShaderMap[ShaderType::eSHIP].LoadFromFiles("resources/shaders/Ship.vert",
                                              "resources/shaders/Ship.frag");
  mShaderMap[ShaderType::ePARTICLE].LoadFromFiles("resources/shaders/Particle.vert",
                                                  "resources/shaders/Particle.frag");
  mShaderMap[ShaderType::eHITBOX].LoadFromFiles("resources/shaders/Hitbox.vert",
                                                "resources/shaders/Hitbox.frag");
}

} // namespace StarBear
