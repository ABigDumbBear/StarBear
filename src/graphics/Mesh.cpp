#include "Mesh.hpp"

namespace StarBear {

/******************************************************************************/
Mesh::Mesh()
{
  // Generate a VAO and several VBOs for the mesh, then configure them.
  glGenVertexArrays(1, &mVertexArray);
  glGenBuffers(1, &mVertexBuffer);
  glGenBuffers(1, &mInstanceBuffer);
  glGenBuffers(1, &mElementBuffer);

  // Bind the vertex array.
  glBindVertexArray(mVertexArray);

  // Bind the vertex buffer.
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);

  // Configure the vertex position attribute.
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(MeshVertex),
                        (void*)(0));

  // Configure the vertex color attribute.
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(MeshVertex),
                        (void*)(offsetof(MeshVertex, mColor)));

  // Configure the vertex normal attribute.
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2,
                        3,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(MeshVertex),
                        (void*)(offsetof(MeshVertex, mNormal)));

  // Configure the vertex texture coordinate attribute.
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3,
                        2,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(MeshVertex),
                        (void*)(offsetof(MeshVertex, mTexCoords)));

  // Bind the instance buffer.
  glBindBuffer(GL_ARRAY_BUFFER, mInstanceBuffer);

  // Configure the instance attributes.
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4,
                        4,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(Mat4),
                        (void*)0);
  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5,
                        4,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(Mat4),
                        (void*)(4 * sizeof(float)));
  glEnableVertexAttribArray(6);
  glVertexAttribPointer(6,
                        4,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(Mat4),
                        (void*)(8 * sizeof(float)));
  glEnableVertexAttribArray(7);
  glVertexAttribPointer(7,
                        4,
                        GL_FLOAT,
                        GL_FALSE,
                        sizeof(Mat4),
                        (void*)(12 * sizeof(float)));

  glVertexAttribDivisor(4, 1);
  glVertexAttribDivisor(5, 1);
  glVertexAttribDivisor(6, 1);
  glVertexAttribDivisor(7, 1);

  // Bind the element buffer.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBuffer);

  // Unbind the vertex array.
  glBindVertexArray(0);

  mValid = true;
}

/******************************************************************************/
Mesh::~Mesh()
{
  if(mValid)
  {
    glDeleteVertexArrays(1, &mVertexArray);
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mInstanceBuffer);
    glDeleteBuffers(1, &mElementBuffer);
  }
}

/******************************************************************************/
Mesh::Mesh(Mesh&& aMesh)
{
  mVertices = aMesh.mVertices;
  mIndices = aMesh.mIndices;

  mVertexArray = aMesh.mVertexArray;
  mVertexBuffer = aMesh.mVertexBuffer;
  mInstanceBuffer = aMesh.mInstanceBuffer;
  mElementBuffer = aMesh.mElementBuffer;

  aMesh.mValid = false;
  mValid = true;
}

/******************************************************************************/
Mesh& Mesh::operator=(Mesh&& aMesh)
{
  mVertices = aMesh.mVertices;
  mIndices = aMesh.mIndices;

  mVertexArray = aMesh.mVertexArray;
  mVertexBuffer = aMesh.mVertexBuffer;
  mInstanceBuffer = aMesh.mInstanceBuffer;
  mElementBuffer = aMesh.mElementBuffer;

  aMesh.mValid = false;
  mValid = true;

  return *this;
}

/******************************************************************************/
void Mesh::Draw(const Shader& aShader, GLenum aMode) const
{
  glBindVertexArray(mVertexArray);

  // Draw the mesh.
  aShader.Use();
  glDrawElements(aMode,
                 mIndices.size(),
                 GL_UNSIGNED_INT,
                 0);

  glBindVertexArray(0);
}

/******************************************************************************/
void Mesh::DrawInstanced(const Shader& aShader,
                         int aNumInstances,
                         GLenum aMode) const
{
  glBindVertexArray(mVertexArray);

  // Draw the mesh.
  aShader.Use();
  glDrawElementsInstanced(aMode,
                          mIndices.size(),
                          GL_UNSIGNED_INT,
                          0,
                          aNumInstances);

  glBindVertexArray(0);
}

/******************************************************************************/
void Mesh::UpdateVertices()
{
  glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
  glBufferData(GL_ARRAY_BUFFER,
               mVertices.size() * sizeof(MeshVertex),
               &mVertices[0],
               GL_STATIC_DRAW);
}

/******************************************************************************/
void Mesh::UpdateIndices()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mElementBuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               mIndices.size() * sizeof(unsigned int),
               &mIndices[0],
               GL_STATIC_DRAW);
}

/******************************************************************************/
void Mesh::InitQuad()
{
  MeshVertex vertex;

  vertex.mPosition = Vec3(0, 0, 0);
  mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(0.1, 0, 0);
  mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(0.1, 0.1, 0);
  mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(0, 0.1, 0);
  mVertices.emplace_back(vertex);

  mIndices.emplace_back(0);
  mIndices.emplace_back(1);
  mIndices.emplace_back(3);
  mIndices.emplace_back(3);
  mIndices.emplace_back(1);
  mIndices.emplace_back(2);

  UpdateVertices();
  UpdateIndices();
}

/******************************************************************************/
void Mesh::InitCube()
{
  MeshVertex vertex;

  // Front face
  vertex.mPosition = Vec3(0, 0, 1);
  vertex.mNormal = Vec3(0, 0, -1);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 1;
  mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(1, 0, 1);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 1;
  mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(1, 1, 1);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 0;
  mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(0, 1, 1);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 0;
  mVertices.emplace_back(vertex);

  mIndices.emplace_back(0);
  mIndices.emplace_back(1);
  mIndices.emplace_back(3);
  mIndices.emplace_back(1);
  mIndices.emplace_back(2);
  mIndices.emplace_back(3);

  // Top face
  vertex.mPosition = Vec3(0, 1, 1);
  vertex.mNormal = Vec3(0, 1, 0);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 1;
  mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(1, 1, 1);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 1;
  mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(1, 1, 0);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 0;
  mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(0, 1, 0);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 0;
  mVertices.emplace_back(vertex);

  mIndices.emplace_back(4);
  mIndices.emplace_back(5);
  mIndices.emplace_back(7);
  mIndices.emplace_back(5);
  mIndices.emplace_back(6);
  mIndices.emplace_back(7);

  // Back face
  vertex.mPosition = Vec3(0, 0, 0);
  vertex.mNormal = Vec3(0, 0, 1);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 1;
  mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(1, 0, 0);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 1;
  mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(1, 1, 0);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 0;
  mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(0, 1, 0);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 0;
  mVertices.emplace_back(vertex);

  mIndices.emplace_back(8);
  mIndices.emplace_back(9);
  mIndices.emplace_back(11);
  mIndices.emplace_back(9);
  mIndices.emplace_back(10);
  mIndices.emplace_back(11);

  // Left face
  vertex.mPosition = Vec3(0, 0, 0);
  vertex.mNormal = Vec3(-1, 0, 0);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 1;
  mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(0, 0, 1);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 1;
  mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(0, 1, 1);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 0;
  mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(0, 1, 0);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 0;
  mVertices.emplace_back(vertex);

  mIndices.emplace_back(12);
  mIndices.emplace_back(13);
  mIndices.emplace_back(15);
  mIndices.emplace_back(13);
  mIndices.emplace_back(14);
  mIndices.emplace_back(15);

  // Bottom face
  vertex.mPosition = Vec3(0, 0, 0);
  vertex.mNormal = Vec3(0, -1, 0);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 1;
  mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(1, 0, 0);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 1;
  mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(1, 0, 1);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 0;
  mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(0, 0, 1);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 0;
  mVertices.emplace_back(vertex);

  mIndices.emplace_back(16);
  mIndices.emplace_back(17);
  mIndices.emplace_back(19);
  mIndices.emplace_back(17);
  mIndices.emplace_back(18);
  mIndices.emplace_back(19);

  // Right face
  vertex.mPosition = Vec3(1, 0, 1);
  vertex.mNormal = Vec3(1, 0, 0);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 1;
  mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(1, 0, 0);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 1;
  mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(1, 1, 0);
  vertex.mTexCoords[0] = 1;
  vertex.mTexCoords[1] = 0;
  mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(1, 1, 1);
  vertex.mTexCoords[0] = 0;
  vertex.mTexCoords[1] = 0;
  mVertices.emplace_back(vertex);

  mIndices.emplace_back(20);
  mIndices.emplace_back(21);
  mIndices.emplace_back(23);
  mIndices.emplace_back(21);
  mIndices.emplace_back(22);
  mIndices.emplace_back(23);

  UpdateVertices();
  UpdateIndices();
}

} // namespace StarBear
