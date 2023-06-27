#include "LaserSystem.hpp"

#include "Hitbox.hpp"
#include "Laser.hpp"
#include "Transform.hpp"

namespace StarBear {

/******************************************************************************/
void LaserSystem::Update(Scene &aScene, double dt) {
  std::set<Entity> deadLasers;
  for (const auto &entity : mEntities) {
    auto &laser = aScene.GetComponentForEntity<Laser>(entity);
    laser.mLifetime -= dt;
    if (laser.mLifetime <= 0) {
      deadLasers.insert(entity);
    }

    auto &hitbox = aScene.GetComponentForEntity<Hitbox>(entity);
    if (hitbox.mCollided) {
      deadLasers.insert(entity);
    }
  }

  for (const auto &entity : deadLasers) {
    aScene.DestroyEntity(entity);
  }
}

/******************************************************************************/
void LaserSystem::Render(Scene &aScene, ResourceMap &aMap, const Mat4 &aView,
                         const Mat4 &aProj) {
  std::vector<Mat4> modelMatrices;
  for (const auto &entity : mEntities) {
    auto &transform = aScene.GetComponentForEntity<Transform>(entity);
    modelMatrices.emplace_back(transform.GetMatrix());
  }

  auto &mesh = aMap.GetMesh(MeshType::eCUBE);
  auto &shader = aMap.GetShader(ShaderType::ePARTICLE);

  glBindBuffer(GL_ARRAY_BUFFER, mesh.GetInstanceBufferID());
  glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(Mat4),
               modelMatrices.data(), GL_DYNAMIC_DRAW);

  shader.Use();
  shader.SetMat4("viewMatrix", aView);
  shader.SetMat4("projectionMatrix", aProj);

  mesh.DrawInstanced(shader, modelMatrices.size());
}

} // namespace StarBear
