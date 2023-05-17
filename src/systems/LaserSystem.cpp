#include "LaserSystem.hpp"

#include "Hitbox.hpp"
#include "Laser.hpp"
#include "Transform.hpp"

namespace StarBear {

/******************************************************************************/
void LaserSystem::Update(Scene& aScene)
{
  std::set<Entity> deadLasers;
  for(const auto& entity : mEntities)
  {
    auto& transform = aScene.GetComponentForEntity<Transform>(entity);

    auto newPos = transform.GetPosition();
    newPos.z -= 3;
    transform.SetPosition(newPos);

    if(newPos.z < -800)
    {
      deadLasers.insert(entity);
    }

    auto& hitbox = aScene.GetComponentForEntity<Hitbox>(entity);
    if(hitbox.mCollided)
    {
      deadLasers.insert(entity);
    }
  }

  for(const auto& entity : deadLasers)
  {
    aScene.DestroyEntity(entity);
  }
}

/******************************************************************************/
void LaserSystem::Render(Scene& aScene, ResourceMap& aMap)
{
  std::vector<Mat4> modelMatrices;
  for(const auto& entity : mEntities)
  {
    auto& transform = aScene.GetComponentForEntity<Transform>(entity);
    modelMatrices.emplace_back(transform.GetMatrix());
  }

  auto& mesh = aMap.GetMesh(MeshType::eCUBE);
  auto& shader = aMap.GetShader(ShaderType::ePARTICLE);

  glBindBuffer(GL_ARRAY_BUFFER, mesh.GetInstanceBufferID());
  glBufferData(GL_ARRAY_BUFFER,
               modelMatrices.size() * sizeof(Mat4),
               modelMatrices.data(),
               GL_DYNAMIC_DRAW);

  shader.Use();
  shader.SetMat4("viewMatrix", View(Vec3(0, 0, 1), Vec3(1, 0, 0), Vec3(0, 0, 50)));
  shader.SetMat4("projectionMatrix", Perspective(45, 1280, 720, 0.1, 1000));

  mesh.DrawInstanced(shader, modelMatrices.size());
}

} // namespace StarBear
