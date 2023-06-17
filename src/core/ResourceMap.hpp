#ifndef RESOURCEMAP_HPP
#define RESOURCEMAP_HPP

#include <map>

#include "Mesh.hpp"
#include "Model.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace StarBear {

enum class MeshType
{
  eQUAD,
  eCUBE
};

enum class ModelType
{
  eBOB,
  eCHALLENGER,
  eDISPATCHER,
  eEXECUTIONER,
  eIMPERIAL,
  eINSURGENT,
  eOMEN,
  ePANCAKE,
  eSPITFIRE,
  eSTRIKER,
  eZENITH
};

enum class ShaderType
{
  eSHIP,
  ePARTICLE,
  eHITBOX
};

enum class TextureType
{
  eCROSSHAIR,
  ePARTICLE
};

class ResourceMap
{
  public:
    ResourceMap();

    Mesh& GetMesh(MeshType aType) { return mMeshMap.at(aType); }
    Model& GetModel(ModelType aType) { return mModelMap.at(aType); }
    Shader& GetShader(ShaderType aType) { return mShaderMap.at(aType); }
    Texture& GetTexture(TextureType aType) { return mTextureMap.at(aType); }

  private:
    std::map<MeshType, Mesh> mMeshMap;
    std::map<TextureType, Texture> mTextureMap;
    std::map<ModelType, Model> mModelMap;
    std::map<ShaderType, Shader> mShaderMap;
};

} // namespace StarBear

#endif
