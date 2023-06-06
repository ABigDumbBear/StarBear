#include "ParentSystem.hpp"

#include "Parent.hpp"
#include "Transform.hpp"

namespace StarBear {

/******************************************************************************/
void CombineTransforms(Scene& aScene, Entity aEntity)
{
  auto& transform = aScene.GetComponentForEntity<Transform>(aEntity);
  auto& children = aScene.GetComponentForEntity<Parent>(aEntity).mChildren;

  for(const auto& child : children)
  {
    auto& childTransform = aScene.GetComponentForEntity<Transform>(child);
    childTransform.Combine(transform);

    childTransform.mTempPosition = (transform.GetRotationMatrix() * childTransform.GetPosition());
    childTransform.mTempPosition = transform.GetTranslationMatrix() * childTransform.mTempPosition;

    if(aScene.DoesEntityHaveComponent<Parent>(child))
    {
      CombineTransforms(aScene, child);
    }
  }
}

/******************************************************************************/
void ParentSystem::Update(Scene& aScene)
{
  for(const auto& entity : mEntities)
  {
    CombineTransforms(aScene, entity);
  }
}

} // namespace StarBear
