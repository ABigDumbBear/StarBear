#include "CollisionSystem.hpp"

#include <KumaGL/Mat4.hpp>
#include <KumaGL/Transform.hpp>

namespace StarBear {

/******************************************************************************/
void CollisionSystem::Update(KumaECS::Scene &aScene) {
  for (const auto &entity : mEntities) {
    auto &hitbox = aScene.GetComponentForEntity<Hitbox>(entity);
    hitbox.mCollided = false;
  }

  for (const auto &entityA : mEntities) {
    for (const auto &entityB : mEntities) {
      if (entityA == entityB) {
        continue;
      }

      auto &transformA =
          aScene.GetComponentForEntity<KumaGL::Transform>(entityA);
      auto posA = transformA.GetMatrix() * KumaGL::Vec3(0, 0, 0);
      auto &hitboxA = aScene.GetComponentForEntity<Hitbox>(entityA);

      auto &transformB =
          aScene.GetComponentForEntity<KumaGL::Transform>(entityB);
      auto posB = transformB.GetMatrix() * KumaGL::Vec3(0, 0, 0);
      auto &hitboxB = aScene.GetComponentForEntity<Hitbox>(entityB);

      if (!hitboxB.mCollided) {
        hitboxB.mCollided = CheckCollision(posA, hitboxA, posB, hitboxB);
      }
    }
  }
}

/******************************************************************************/
void CollisionSystem::Render(KumaECS::Scene &aScene, ResourceMap &aMap,
                             const KumaGL::Mat4 &aView,
                             const KumaGL::Mat4 &aProj) {
  std::vector<KumaGL::Mat4> modelMatrices;
  for (const auto &entity : mEntities) {
    auto &transform = aScene.GetComponentForEntity<KumaGL::Transform>(entity);
    auto &hitbox = aScene.GetComponentForEntity<Hitbox>(entity);

    KumaGL::Vec3 pos(hitbox.x + (hitbox.mWidth / 2.0),
                     hitbox.y + (hitbox.mHeight / 2.0),
                     hitbox.z + (hitbox.mDepth / 2.0));
    pos = transform.GetWorldPosition() + pos;

    auto mat = Translate(pos);
    mat =
        mat * Scale(KumaGL::Vec3(hitbox.mWidth, hitbox.mHeight, hitbox.mDepth));
    modelMatrices.emplace_back(mat);
  }

  auto &mesh = aMap.GetMesh(MeshType::eCUBE);
  auto &shader = aMap.GetShader(ShaderType::eHITBOX);

  glBindBuffer(GL_ARRAY_BUFFER, mesh.GetInstanceBufferID());
  glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(KumaGL::Mat4),
               modelMatrices.data(), GL_DYNAMIC_DRAW);

  shader.Use();
  shader.SetMat4("viewMatrix", aView);
  shader.SetMat4("projectionMatrix", aProj);

  mesh.DrawInstanced(modelMatrices.size(), GL_LINE_LOOP);
}

/******************************************************************************/
bool CollisionSystem::CheckCollision(const KumaGL::Vec3 &aPositionA,
                                     const Hitbox &aHitboxA,
                                     const KumaGL::Vec3 &aPositionB,
                                     const Hitbox &aHitboxB) {
  bool collision = false;

  auto leftA = aPositionA.x + aHitboxA.x;
  auto rightA = leftA + aHitboxA.mWidth;
  auto bottomA = aPositionA.y + aHitboxA.y;
  auto topA = bottomA + aHitboxA.mHeight;
  auto farA = aPositionA.z + aHitboxA.z;
  auto nearA = farA + aHitboxA.mDepth;

  auto leftB = aPositionB.x + aHitboxB.x;
  auto rightB = leftB + aHitboxB.mWidth;
  auto bottomB = aPositionB.y + aHitboxB.y;
  auto topB = bottomB + aHitboxB.mHeight;
  auto farB = aPositionB.z + aHitboxB.z;
  auto nearB = farB + aHitboxB.mDepth;

  if (rightA >= leftB && rightB >= leftA) {
    if (topA >= bottomB && topB >= bottomA) {
      if (nearA >= farB && nearB >= farA) {
        collision = true;
      }
    }
  }

  return collision;
}

} // namespace StarBear
