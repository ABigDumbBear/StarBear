#include "Shader.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

namespace KumaGL {

/******************************************************************************/
Shader::Shader()
{
  mID = glCreateProgram();
}

/******************************************************************************/
Shader::~Shader()
{
  glDeleteProgram(mID);
}

/******************************************************************************/
void Shader::LoadFromFiles(const std::string& aVertexFile,
                           const std::string& aFragmentFile)
{
  std::ifstream vertexInput, fragmentInput;
  vertexInput.open(aVertexFile);
  fragmentInput.open(aFragmentFile);

  std::stringstream vertexStream, fragmentStream;
  vertexStream << vertexInput.rdbuf();
  fragmentStream << fragmentInput.rdbuf();

  vertexInput.close();
  fragmentInput.close();

  LoadFromSource(vertexStream.str(), fragmentStream.str());
}

/******************************************************************************/
void Shader::LoadFromSource(const std::string& aVertexSource,
                            const std::string& aFragmentSource)
{
  // First, compile the shaders.
  unsigned int vertexID, fragmentID;
  CompileShader(vertexID, aVertexSource, ShaderType::eVERTEX);
  CompileShader(fragmentID, aFragmentSource, ShaderType::eFRAGMENT);

  // Then, create and link the shader program.
  LinkProgram(vertexID, fragmentID);

  // Finally, delete the shaders since they have been linked into the
  // program and are no longer needed.
  glDeleteShader(vertexID);
  glDeleteShader(fragmentID);
}

/******************************************************************************/
void Shader::Use() const
{
  glUseProgram(mID);
}

/******************************************************************************/
void Shader::SetInt(const std::string& aName, int aValue) const
{
  int loc = glGetUniformLocation(mID, aName.c_str());
  glUniform1i(loc, aValue);
}

/******************************************************************************/
void Shader::SetFloat(const std::string& aName, float aValue) const
{
  int loc = glGetUniformLocation(mID, aName.c_str());
  glUniform1f(loc, aValue);
}

/******************************************************************************/
void Shader::SetVec3(const std::string& aName, const Vec3& aValue) const
{
  int loc = glGetUniformLocation(mID, aName.c_str());
  glUniform3fv(loc, 1, &aValue.x);
}

/******************************************************************************/
void Shader::SetMat4(const std::string& aName, const Mat4& aValue) const
{
  int loc = glGetUniformLocation(mID, aName.c_str());
  glUniformMatrix4fv(loc, 1, GL_FALSE, &aValue(0, 0));
}

/******************************************************************************/
void Shader::CompileShader(unsigned int& aID,
                           const std::string& aSource,
                           ShaderType aType)
{
  const char* source = aSource.c_str();

  switch(aType)
  {
    case ShaderType::eVERTEX: { aID = glCreateShader(GL_VERTEX_SHADER); break; }
    case ShaderType::eFRAGMENT: { aID = glCreateShader(GL_FRAGMENT_SHADER); break; }
  }

  glShaderSource(aID, 1, &source, NULL);
  glCompileShader(aID);

  // Check for compile errors.
  int success;
  char infoLog[512];
  glGetShaderiv(aID, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(aID, 512, NULL, infoLog);
    std::cout << "Error compiling shader!\n" << infoLog << std::endl;
  }
}

/******************************************************************************/
void Shader::LinkProgram(unsigned int aVertexID,
                         unsigned int aFragmentID)
{
  glAttachShader(mID, aVertexID);
  glAttachShader(mID, aFragmentID);
  glLinkProgram(mID);

  // Check for linking errors.
  int success;
  char infoLog[512];
  glGetProgramiv(mID, GL_LINK_STATUS, &success);
  if(!success)
  {
    glGetProgramInfoLog(mID, 512, NULL, infoLog);
    std::cout << "Error linking program!\n" << infoLog << std::endl;
  }
}

} // namespace KumaGL
