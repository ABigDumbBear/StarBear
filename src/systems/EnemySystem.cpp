#include "EnemySystem.hpp"

#include <cmath>
#include <set>

#include "Hitbox.hpp"
#include "Enemy.hpp"
#include "Transform.hpp"

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
void EnemySystem::Update(Scene& aScene, double dt)
{
  mTimer += dt;

  std::set<Entity> deadEntities;
  for(const auto& entity : mEntities)
  {
    auto& transform = aScene.GetComponentForEntity<Transform>(entity);
    //transform.SetRotation(0, 0, easeInOutBack(mTimer) * 360);

    auto& enemy = aScene.GetComponentForEntity<Enemy>(entity);
    auto& hitbox = aScene.GetComponentForEntity<Hitbox>(entity);
    if(hitbox.mCollided)
    {
      --enemy.mHealth;
      if(enemy.mHealth <= 0)
      {
        deadEntities.insert(entity);
      }
    }
  }

  for(const auto& entity : deadEntities)
  {
    aScene.DestroyEntity(entity);
  }

  if(mTimer > 1)
  {
    mTimer = 0;
  }
}

/******************************************************************************/
void EnemySystem::Render(Scene& aScene, ResourceMap& aMap)
{
  // Store the model matrix for each ship.
  std::vector<Mat4> modelMatrices;
  std::vector<float> inverts;
  for(const auto& entity : mEntities)
  {
    auto& transform = aScene.GetComponentForEntity<Transform>(entity);
    modelMatrices.emplace_back(transform.GetMatrix());

    auto& hitbox = aScene.GetComponentForEntity<Hitbox>(entity);
    inverts.emplace_back((float)hitbox.mCollided);
  }

  // For each mesh in the model, bind the mesh's instance buffer and
  // write the model matrices into it.
  auto& model = aMap.GetModel(ModelType::eBOB);
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
