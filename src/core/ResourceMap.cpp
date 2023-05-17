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

  // Load models.
  mModelMap.emplace(ModelType::eBOB, Model());
  mModelMap.emplace(ModelType::eSPITFIRE, Model());

  mModelMap[ModelType::eBOB].LoadFromFile("resources/models/Bob/OBJ/Bob.obj");
  mModelMap[ModelType::eSPITFIRE].LoadFromFile("resources/models/Spitfire/OBJ/Spitfire.obj");

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
