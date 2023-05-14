#include "CollisionSystem.hpp"

namespace StarBear {

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
    auto& transformA = aScene.GetComponentForEntity<Transform>(entityA);
    auto& hitboxA = aScene.GetComponentForEntity<Hitbox>(entityA);
    for(const auto& entityB : mEntities)
    {
      if(entityA == entityB) { continue; }

      auto& transformB = aScene.GetComponentForEntity<Transform>(entityB);
      auto& hitboxB = aScene.GetComponentForEntity<Hitbox>(entityB);

      hitboxB.mCollided = CheckCollision(transformA, hitboxA, transformB, hitboxB);
    }
  }
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

  auto leftB = aTransformB.GetPosition().x + aHitboxB.x;
  auto rightB = leftB + aHitboxB.mWidth;
  auto bottomB = aTransformB.GetPosition().y + aHitboxB.y;
  auto topB = bottomB + aHitboxB.mHeight;

  if(rightA >= leftB && rightB >= leftA)
  {
    if(topA >= bottomB && topB >= bottomA)
    {
      collision = true;
    }
  }

  return collision;
}

} // namespace StarBear
