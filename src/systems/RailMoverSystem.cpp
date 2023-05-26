#include "RailMoverSystem.hpp"

#include "RailMover.hpp"
#include "Transform.hpp"

namespace StarBear {

/******************************************************************************/
void RailMoverSystem::Update(Scene& aScene)
{
  for(const auto& entity : mEntities)
  {
    auto& railMover = aScene.GetComponentForEntity<RailMover>(entity);
    auto& transform = aScene.GetComponentForEntity<Transform>(entity);

    transform.Translate(Vec3(0, 0, -0.1));

    for(const auto& child : railMover.mChildren)
    {
      auto& childTransform = aScene.GetComponentForEntity<Transform>(child);
      childTransform.Combine(transform);
    }
  }
}

} // namespace StarBear
