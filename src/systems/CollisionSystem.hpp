#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP

#include "ECS.hpp"

#include "ResourceMap.hpp"

#include "Hitbox.hpp"
#include "Transform.hpp"

namespace StarBear {

class CollisionSystem : public System
{
  public:
    void Update(Scene& aScene);
    void Render(Scene& aScene, ResourceMap& aMap);

  private:
    bool CheckCollision(const Transform& aTransformA,
                        const Hitbox& aHitboxA,
                        const Transform& aTransformB,
                        const Hitbox& aHitboxB);
};

} // namespace StarBear

#endif
