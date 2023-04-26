#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include <glad/glad.h>

#include "Vec3.hpp"

#include "Shader.hpp"
#include "Texture.hpp"

#include "Mat4.hpp"

namespace KumaGL {

struct MeshVertex
{
  Vec3 mPosition;
  Vec3 mColor;
  Vec3 mNormal;
  float mTexCoords[2] { 0.0, 0.0 };
};

class Mesh
{
  public:
    Mesh();
    ~Mesh();
    Mesh(const Mesh& aMesh) = delete;
    Mesh& operator=(const Mesh& aMesh) = delete;
    Mesh(Mesh&&) = delete;
    Mesh& operator=(Mesh&& aMesh) = delete;

    void Draw(const Shader& aShader,
              GLenum aMode = GL_TRIANGLES) const;
    void DrawInstanced(const Shader& aShader,
                       int aNumInstances,
                       GLenum aMode = GL_TRIANGLES) const;

    void UpdateVertices();
    void UpdateIndices();

    GLuint GetVertexArrayID() const { return mVertexArray; }
    GLuint GetVertexBufferID() const { return mVertexBuffer; }
    GLuint GetInstanceBufferID() const { return mInstanceBuffer; }
    GLuint GetElementBufferID() const { return mElementBuffer; }

    std::vector<MeshVertex> mVertices;
    std::vector<unsigned int> mIndices;

  private:
    GLuint mVertexArray { 0 };
    GLuint mVertexBuffer { 0 };
    GLuint mInstanceBuffer { 0 };
    GLuint mElementBuffer { 0 };
};

} // namespace KumaGL

#endif
