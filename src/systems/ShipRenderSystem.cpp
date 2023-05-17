#include "ShipRenderSystem.hpp"

#include <vector>

#include "Mat4.hpp"
#include "Transform.hpp"

namespace StarBear {

/******************************************************************************/
void ShipRenderSystem::Render(Scene& aScene, ResourceMap& aMap)
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
  auto& model = aMap.GetModel(ModelType::eSPITFIRE);
  for(const auto& mesh : model.GetMeshes())
  {
    glBindBuffer(GL_ARRAY_BUFFER, mesh.GetInstanceBufferID());
    glBufferData(GL_ARRAY_BUFFER,
                 modelMatrices.size() * sizeof(Mat4),
                 modelMatrices.data(),
                 GL_DYNAMIC_DRAW);
  }

  // Set shader uniforms and draw the model.
  auto& shader = aMap.GetShader(ShaderType::eSHIP);
  shader.Use();
  shader.SetMat4("viewMatrix", View(Vec3(0, 0, 1), Vec3(1, 0, 0), Vec3(0, 0, 50)));
  shader.SetMat4("projectionMatrix", Perspective(45, 1280, 720, 0.1, 1000));

  model.DrawInstanced(shader, mEntities.size());
}

} // namespace StarBear
