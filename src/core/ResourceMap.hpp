#ifndef RESOURCEMAP_HPP
#define RESOURCEMAP_HPP

#include <map>

#include <KumaGL/Mesh.hpp>
#include <KumaGL/Model.hpp>
#include <KumaGL/Shader.hpp>
#include <KumaGL/Texture.hpp>

namespace StarBear {

enum class MeshType { eQUAD, eCUBE };

enum class ModelType { eBOB, eSPITFIRE };

enum class ShaderType { eSHIP, ePARTICLE, eHITBOX };

enum class TextureType { eCROSSHAIR, ePARTICLE };

class ResourceMap {
public:
  ResourceMap();

  KumaGL::Mesh &GetMesh(MeshType aType) { return mMeshMap.at(aType); }
  KumaGL::Model &GetModel(ModelType aType) { return mModelMap.at(aType); }
  KumaGL::Shader &GetShader(ShaderType aType) { return mShaderMap.at(aType); }
  KumaGL::Texture &GetTexture(TextureType aType) {
    return mTextureMap.at(aType);
  }

private:
  std::map<MeshType, KumaGL::Mesh> mMeshMap;
  std::map<TextureType, KumaGL::Texture> mTextureMap;
  std::map<ModelType, KumaGL::Model> mModelMap;
  std::map<ShaderType, KumaGL::Shader> mShaderMap;
};

} // namespace StarBear

#endif
