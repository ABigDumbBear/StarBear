#include "ShipControllerSystem.hpp"

#include "Transform.hpp"

namespace StarBear {

/******************************************************************************/
void ShipControllerSystem::Update(Scene& aScene)
{
  for(const auto& entity : mEntities)
  {
    auto& transform = aScene.GetComponentForEntity<Transform>(entity);
    transform.Rotate(1, 0, 0);
  }
}

} // namespace StarBear
