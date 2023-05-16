#include "LaserSystem.hpp"

#include "Hitbox.hpp"
#include "Laser.hpp"
#include "Transform.hpp"

namespace StarBear {

/******************************************************************************/
LaserSystem::LaserSystem()
{
  mMesh.InitCube();
  mShader.LoadFromFiles("resources/shaders/Particle.vert",
                        "resources/shaders/Particle.frag");
}

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
void LaserSystem::Render(Scene& aScene)
{
  std::vector<Mat4> modelMatrices;
  for(const auto& entity : mEntities)
  {
    auto& transform = aScene.GetComponentForEntity<Transform>(entity);
    modelMatrices.emplace_back(transform.GetMatrix());
  }

  glBindBuffer(GL_ARRAY_BUFFER, mMesh.GetInstanceBufferID());
  glBufferData(GL_ARRAY_BUFFER,
               modelMatrices.size() * sizeof(Mat4),
               modelMatrices.data(),
               GL_DYNAMIC_DRAW);

  mShader.Use();
  mShader.SetMat4("viewMatrix", View(Vec3(0, 0, 1), Vec3(1, 0, 0), Vec3(0, 0, 50)));
  mShader.SetMat4("projectionMatrix", Perspective(45, 1280, 720, 0.1, 1000));

  mMesh.DrawInstanced(mShader, modelMatrices.size());
}

} // namespace StarBear
