#include "CollisionSystem.hpp"

#include "Mat4.hpp"

namespace StarBear {

/******************************************************************************/
CollisionSystem::CollisionSystem()
{
  mMesh.InitCube();
  mShader.LoadFromFiles("resources/shaders/Hitbox.vert",
                        "resources/shaders/Hitbox.frag");
}

/******************************************************************************/
void CollisionSystem::Update(Scene& aScene)
{
  for(const auto& entity : mEntities)
  {
    auto& hitbox = aScene.GetComponentForEntity<Hitbox>(entity);
    hitbox.mCollided = false;
  }

  for(const auto& entityA : mEntities)
  {
    for(const auto& entityB : mEntities)
    {
      if(entityA == entityB) { continue; }

      auto& transformA = aScene.GetComponentForEntity<Transform>(entityA);
      auto& hitboxA = aScene.GetComponentForEntity<Hitbox>(entityA);
      auto& transformB = aScene.GetComponentForEntity<Transform>(entityB);
      auto& hitboxB = aScene.GetComponentForEntity<Hitbox>(entityB);

      if(!hitboxB.mCollided)
      {
        hitboxB.mCollided = CheckCollision(transformA, hitboxA, transformB, hitboxB);
      }
    }
  }
}

/******************************************************************************/
void CollisionSystem::Render(Scene& aScene)
{
  std::vector<Mat4> modelMatrices;
  for(const auto& entity : mEntities)
  {
    auto& transform = aScene.GetComponentForEntity<Transform>(entity);
    auto& hitbox = aScene.GetComponentForEntity<Hitbox>(entity);

    auto targetPos = transform.GetPosition();
    targetPos.x += hitbox.x;
    targetPos.y += hitbox.y;
    targetPos.z += hitbox.z;

    auto mat = Translate(targetPos);
    mat = mat * Scale(Vec3(hitbox.mWidth, hitbox.mHeight, hitbox.mDepth));
    modelMatrices.emplace_back(mat);
  }

  glBindBuffer(GL_ARRAY_BUFFER, mMesh.GetInstanceBufferID());
  glBufferData(GL_ARRAY_BUFFER,
               modelMatrices.size() * sizeof(Mat4),
               modelMatrices.data(),
               GL_DYNAMIC_DRAW);

  mShader.Use();
  mShader.SetMat4("viewMatrix", View(Vec3(0, 0, 1), Vec3(1, 0, 0), Vec3(0, 0, 50)));
  mShader.SetMat4("projectionMatrix", Perspective(45, 1280, 720, 0.1, 1000));

  mMesh.DrawInstanced(mShader, modelMatrices.size(), GL_LINE_LOOP);
}

/******************************************************************************/
bool CollisionSystem::CheckCollision(const Transform& aTransformA,
                                     const Hitbox& aHitboxA,
                                     const Transform& aTransformB,
                                     const Hitbox& aHitboxB)
{
  bool collision = false;

  auto leftA = aTransformA.GetPosition().x + aHitboxA.x;
  auto rightA = leftA + aHitboxA.mWidth;
  auto bottomA = aTransformA.GetPosition().y + aHitboxA.y;
  auto topA = bottomA + aHitboxA.mHeight;
  auto farA = aTransformA.GetPosition().z + aHitboxA.z;
  auto nearA = farA + aHitboxA.mDepth;

  auto leftB = aTransformB.GetPosition().x + aHitboxB.x;
  auto rightB = leftB + aHitboxB.mWidth;
  auto bottomB = aTransformB.GetPosition().y + aHitboxB.y;
  auto topB = bottomB + aHitboxB.mHeight;
  auto farB = aTransformB.GetPosition().z + aHitboxB.z;
  auto nearB = farB + aHitboxB.mDepth;

  if(rightA >= leftB && rightB >= leftA)
  {
    if(topA >= bottomB && topB >= bottomA)
    {
      if(nearA >= farB && nearB >= farA)
      {
        collision = true;
      }
    }
  }

  return collision;
}

} // namespace StarBear
