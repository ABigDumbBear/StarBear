#include "PhysicsSystem.hpp"

#include "Transform.hpp"
#include "Physics.hpp"

namespace StarBear {

/******************************************************************************/
void PhysicsSystem::Update(Scene& aScene, double dt)
{
  for(const auto& entity : mEntities)
  { 
    auto& transform = aScene.GetComponentForEntity<Transform>(entity);
    auto& physics = aScene.GetComponentForEntity<Physics>(entity);

    physics.mVelocity += physics.mAcceleration * dt;
    transform.Translate((physics.mVelocity * dt) + (physics.mAcceleration * dt * dt));
  }
}

} // namespace StarBear
