#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP

#include <KumaECS/ECS.hpp>

#include <KumaGL/Vec3.hpp>

#include "Hitbox.hpp"
#include "ResourceMap.hpp"

namespace StarBear {

class CollisionSystem : public KumaECS::System {
public:
  void Update(KumaECS::Scene &aScene);
  void Render(KumaECS::Scene &aScene, ResourceMap &aMap,
              const KumaGL::Mat4 &aView, const KumaGL::Mat4 &aProj);

private:
  bool CheckCollision(const KumaGL::Vec3 &aPositionA, const Hitbox &aHitboxA,
                      const KumaGL::Vec3 &aPositionB, const Hitbox &aHitboxB);
};

} // namespace StarBear

#endif
