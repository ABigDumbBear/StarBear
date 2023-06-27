#include "CollisionSystem.hpp"

#include "Mat4.hpp"

#include "Laser.hpp"
#include "Transform.hpp"

namespace StarBear {

/******************************************************************************/
void CollisionSystem::Update(Scene &aScene) {
  for (const auto &entity : mEntities) {
    auto &hitbox = aScene.GetComponentForEntity<Hitbox>(entity);
    hitbox.mCollided = false;
  }

  for (const auto &entityA : mEntities) {
    for (const auto &entityB : mEntities) {
      if (entityA == entityB) {
        continue;
      }

      auto &transformA = aScene.GetComponentForEntity<Transform>(entityA);
      auto posA = transformA.GetMatrix() * Vec3(0, 0, 0);
      auto &hitboxA = aScene.GetComponentForEntity<Hitbox>(entityA);

      auto &transformB = aScene.GetComponentForEntity<Transform>(entityB);
      auto posB = transformB.GetMatrix() * Vec3(0, 0, 0);
      auto &hitboxB = aScene.GetComponentForEntity<Hitbox>(entityB);

      if (!hitboxB.mCollided) {
        hitboxB.mCollided = CheckCollision(posA, hitboxA, posB, hitboxB);
      }
    }
  }
}

/******************************************************************************/
void CollisionSystem::Render(Scene &aScene, ResourceMap &aMap,
                             const Mat4 &aView, const Mat4 &aProj) {
  std::vector<Mat4> modelMatrices;
  for (const auto &entity : mEntities) {
    auto &transform = aScene.GetComponentForEntity<Transform>(entity);
    auto &hitbox = aScene.GetComponentForEntity<Hitbox>(entity);

    Vec3 pos(hitbox.x, hitbox.y, hitbox.z);
    pos = transform.GetWorldPosition() + pos;

    auto mat = Translate(pos);
    mat = mat * Scale(Vec3(hitbox.mWidth, hitbox.mHeight, hitbox.mDepth));
    modelMatrices.emplace_back(mat);
  }

  auto &mesh = aMap.GetMesh(MeshType::eCUBE);
  auto &shader = aMap.GetShader(ShaderType::eHITBOX);

  glBindBuffer(GL_ARRAY_BUFFER, mesh.GetInstanceBufferID());
  glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(Mat4),
               modelMatrices.data(), GL_DYNAMIC_DRAW);

  shader.Use();
  shader.SetMat4("viewMatrix", aView);
  shader.SetMat4("projectionMatrix", aProj);

  mesh.DrawInstanced(shader, modelMatrices.size(), GL_LINE_LOOP);
}

/******************************************************************************/
bool CollisionSystem::CheckCollision(const Vec3 &aPositionA,
                                     const Hitbox &aHitboxA,
                                     const Vec3 &aPositionB,
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
