#include "ShipControllerSystem.hpp"

#include <GLFW/glfw3.h>

#include "EntityFactory.hpp"
#include "MathUtil.hpp"

#include "ShipController.hpp"
#include "Transform.hpp"

namespace StarBear {

/******************************************************************************/
void ShipControllerSystem::Update(Scene& aScene, const Input& aInput, double dt)
{
  mTimer += dt;

  if(aInput.mPressedKeys.count(GLFW_KEY_W))
  {
    mTargetPos.y += 1;
  }
  if(aInput.mPressedKeys.count(GLFW_KEY_A))
  {
    mTargetPos.x -= 1;
  }
  if(aInput.mPressedKeys.count(GLFW_KEY_S))
  {
    mTargetPos.y -= 1;
  }
  if(aInput.mPressedKeys.count(GLFW_KEY_D))
  {
    mTargetPos.x += 1;
  }

  if(aInput.mPressedKeys.count(GLFW_KEY_SPACE) && mTimer > 0.1)
  {
    // create a laser
    auto laser = CreateLaser(aScene);
    aScene.GetComponentForEntity<Transform>(laser).SetPosition(mTargetPos);
    aScene.GetComponentForEntity<Physics>(laser).mVelocity.z = -5;

    mTimer = 0;
  }

  for(const auto& entity : mEntities)
  {
    auto& controller = aScene.GetComponentForEntity<ShipController>(entity);
    auto& transform = aScene.GetComponentForEntity<Transform>(entity);

    auto newPos = transform.GetPosition();
    newPos = Lerp(newPos, mTargetPos, 0.3);
    newPos.z = 0;
    transform.SetPosition(newPos);
  }
}

/******************************************************************************/
void ShipControllerSystem::Render(Scene& aScene, ResourceMap& aMap)
{
  // Store the model matrix for each ship.
  std::vector<Mat4> modelMatrices;
  std::vector<float> inverts;
  for(const auto& entity : mEntities)
  {
    auto& transform = aScene.GetComponentForEntity<Transform>(entity);
    modelMatrices.emplace_back(transform.GetMatrix());
    inverts.emplace_back(1);
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

    glBindBuffer(GL_ARRAY_BUFFER, mesh.GetCustomBufferID());
    glBufferData(GL_ARRAY_BUFFER,
                 inverts.size() * sizeof(float),
                 inverts.data(),
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
