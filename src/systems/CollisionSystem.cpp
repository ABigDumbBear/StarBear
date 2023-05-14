#include "CollisionSystem.hpp"

#include <iostream>

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
  auto nearA = aTransformA.GetPosition().z + aHitboxA.z;
  auto farA = nearA + aHitboxA.mDepth;

  std::cout << "leftA: " << leftA << std::endl;
  std::cout << "rightA: " << rightA << std::endl;
  std::cout << "bottomA: " << bottomA << std::endl;
  std::cout << "topA: " << topA << std::endl;
  std::cout << "nearA: " << nearA << std::endl;
  std::cout << "farA: " << farA << std::endl;

  auto leftB = aTransformB.GetPosition().x + aHitboxB.x;
  auto rightB = leftB + aHitboxB.mWidth;
  auto bottomB = aTransformB.GetPosition().y + aHitboxB.y;
  auto topB = bottomB + aHitboxB.mHeight;
  auto nearB = aTransformB.GetPosition().z + aHitboxB.z;
  auto farB = nearB + aHitboxB.mDepth;

  std::cout << "leftB: " << leftB << std::endl;
  std::cout << "rightB: " << rightB << std::endl;
  std::cout << "bottomB: " << bottomB << std::endl;
  std::cout << "topB: " << topB << std::endl;
  std::cout << "nearB: " << nearB << std::endl;
  std::cout << "farB: " << farB << std::endl;

  if(rightA >= leftB && rightB >= leftA)
  {
    std::cout << "x overlap" << std::endl;
    if(topA >= bottomB && topB >= bottomA)
    {
      std::cout << "y overlap" << std::endl;
      if(farA >= nearB && farB >= nearA)
      {
        std::cout << "z overlap" << std::endl;
        collision = true;
      }
    }
  }

  return collision;
}

} // namespace StarBear
