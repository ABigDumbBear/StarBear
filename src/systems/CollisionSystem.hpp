#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP

#include "ECS.hpp"
#include "ResourceMap.hpp"

#include "Vec3.hpp"

#include "Hitbox.hpp"

namespace StarBear {

class CollisionSystem : public System
{
  public:
    void Update(Scene& aScene);
    void Render(Scene& aScene,
                ResourceMap& aMap,
                const Mat4& aView,
                const Mat4& aProj);

  private:
    bool CheckCollision(const Vec3& aPositionA,
                        const Hitbox& aHitboxA,
                        const Vec3& aPositionB,
                        const Hitbox& aHitboxB);
};

} // namespace StarBear

#endif
