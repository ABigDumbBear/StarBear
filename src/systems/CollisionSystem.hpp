#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP

#include "ECS.hpp"

#include "Hitbox.hpp"
#include "Transform.hpp"

namespace StarBear {

class CollisionSystem : public System
{
  public:
    void Update(Scene& aScene);

  private:
    bool CheckCollision(const Transform& aTransformA,
                        const Hitbox& aHitboxA,
                        const Transform& aTransformB,
                        const Hitbox& aHitboxB);
};

} // namespace StarBear

#endif
