#include "EnemySystem.hpp"

#include <cmath>

#include "Hitbox.hpp"
#include "Enemy.hpp"
#include "Transform.hpp"

#include <iostream>

namespace StarBear {

float easeInOutBack(double x)
{
  float c1 = 1.70158;
  float c2 = c1 * 1.525;

  return x < 0.5 ?
         (std::pow(2.0 * x, 2.0) * ((c2 + 1.0) * 2.0 * x - c2)) / 2.0 :
         (std::pow(2.0 * x - 2.0, 2.0) * ((c2 + 1.0) * (x * 2.0 - 2.0) + c2) + 2.0) / 2.0;
}

/******************************************************************************/
EnemySystem::EnemySystem()
{
  mShader.LoadFromFiles("resources/shaders/Ship.vert",
                        "resources/shaders/Ship.frag");
  mModel.LoadFromFile("resources/models/Bob/OBJ/Bob.obj");
}

/******************************************************************************/
void EnemySystem::Update(Scene& aScene, double dt)
{
  mTimer += dt;
  for(const auto& entity : mEntities)
  {
    auto& transform = aScene.GetComponentForEntity<Transform>(entity);
    transform.SetRotation(0, 0, easeInOutBack(mTimer) * 360);
  }

  if(mTimer > 1)
  {
    mTimer = 0;
  }
}

/******************************************************************************/
void EnemySystem::Render(Scene& aScene)
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
  mShader.SetMat4("viewMatrix", View(Vec3(0, 0, 1), Vec3(1, 0, 0), Vec3(0, 0, 50)));
  mShader.SetMat4("projectionMatrix", Perspective(45, 1280, 720, 0.1, 100));

  mModel.DrawInstanced(mShader, mEntities.size());
}

} // namespace StarBear
