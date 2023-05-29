#include "ParentSystem.hpp"

#include "Parent.hpp"
#include "Transform.hpp"

namespace StarBear {

/******************************************************************************/
void ParentSystem::Update(Scene& aScene)
{
  for(const auto& entity : mEntities)
  {
    auto& transform = aScene.GetComponentForEntity<Transform>(entity);
    auto& children = aScene.GetComponentForEntity<Parent>(entity).mChildren;

    for(const auto& child : children)
    {
      auto& childTransform = aScene.GetComponentForEntity<Transform>(child);
      childTransform.Combine(transform);
    }
  }
}

} // namespace StarBear
