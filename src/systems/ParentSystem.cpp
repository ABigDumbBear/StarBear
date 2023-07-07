#include "ParentSystem.hpp"

#include <KumaGL/Transform.hpp>

#include "Parent.hpp"

namespace StarBear {

/******************************************************************************/
void CombineTransforms(KumaECS::Scene &aScene, KumaECS::Entity aEntity) {
  auto &transform = aScene.GetComponentForEntity<KumaGL::Transform>(aEntity);
  auto &children = aScene.GetComponentForEntity<Parent>(aEntity).mChildren;

  for (const auto &child : children) {
    auto &childTransform =
        aScene.GetComponentForEntity<KumaGL::Transform>(child);
    childTransform.Combine(transform);

    if (aScene.DoesEntityHaveComponent<Parent>(child)) {
      CombineTransforms(aScene, child);
    }
  }
}

/******************************************************************************/
void ParentSystem::Update(KumaECS::Scene &aScene) {
  for (const auto &entity : mEntities) {
    CombineTransforms(aScene, entity);
  }
}

} // namespace StarBear
