#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/glad.h>

#include "Mat4.hpp"
#include "Vec3.hpp"

namespace KumaGL {

class Shader
{
  public:
    Shader();
    ~Shader();
    Shader(const Shader& aShader) = delete;
    Shader& operator=(const Shader& aShader) = delete;
    Shader(Shader&&) = delete;
    Shader& operator=(Shader&& aMesh) = delete;

    void LoadFromFiles(const std::string& aVertexFile,
                       const std::string& aFragmentFile);
    void LoadFromSource(const std::string& aVertexSource,
                        const std::string& aFragmentSource);

    void Use() const;

    void SetInt(const std::string& aName, int aValue) const;
    void SetFloat(const std::string& aName, float aValue) const;
    void SetVec3(const std::string& aName, const Vec3& aValue) const;
    void SetMat4(const std::string& aName, const Mat4& aValue) const;

    GLuint GetID() const { return mID; }

  private:
    enum class ShaderType
    {
      eVERTEX,
      eFRAGMENT
    };

    void CompileShader(unsigned int& aID, const std::string& aSource, ShaderType aType);
    void LinkProgram(unsigned int aVertexID, unsigned int aFragmentID);

    GLuint mID { 0 };
};

} // namespace KumaGL

#endif
