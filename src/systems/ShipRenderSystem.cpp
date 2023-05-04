#include "ShipRenderSystem.hpp"

#include <vector>

#include "Mat4.hpp"
#include "Transform.hpp"

namespace StarBear {

/******************************************************************************/
ShipRenderSystem::ShipRenderSystem()
{
  mShader.LoadFromFiles("resources/shaders/Ship.vert",
                        "resources/shaders/Ship.frag");
  mModel.LoadFromFile("resources/models/Spitfire/OBJ/Spitfire.obj");
}

/******************************************************************************/
void ShipRenderSystem::Render(Scene& aScene)
{
  // Store the model matrix for each ship.
  std::vector<Mat4> modelMatrices;
  for(const auto& entity : mEntities)
  {
    auto& transform = aScene.GetComponentForEntity<Transform>(entity);
    modelMatrices.emplace_back(transform.GetMatrix());
  }

  // For each mesh in the model, bind the mesh's instance buffer and
  // write the model matrices into it.
  for(const auto& mesh : mModel.GetMeshes())
  {
    glBindBuffer(GL_ARRAY_BUFFER, mesh.GetInstanceBufferID());
    glBufferData(GL_ARRAY_BUFFER,
                 modelMatrices.size() * sizeof(Mat4),
                 modelMatrices.data(),
                 GL_DYNAMIC_DRAW);
  }

  // Set shader uniforms and draw the model.
  mShader.Use();
  mShader.SetMat4("viewMatrix", View(Vec3(0, 0, 1), Vec3(1, 0, 0), Vec3(0, 0, 30)));
  mShader.SetMat4("projectionMatrix", Perspective(45, 1280, 720, 0.1, 100));

  mModel.DrawInstanced(mShader, mEntities.size());
}

} // namespace StarBear
