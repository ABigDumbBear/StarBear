#include "RailMoverSystem.hpp"

#include <KumaGL/Transform.hpp>

#include "RailMover.hpp"

namespace StarBear {

/******************************************************************************/
void RailMoverSystem::Update(KumaECS::Scene &aScene) {
  for (const auto &entity : mEntities) {
    auto &railMover = aScene.GetComponentForEntity<RailMover>(entity);
    auto &transform = aScene.GetComponentForEntity<KumaGL::Transform>(entity);

    transform.Translate(KumaGL::Vec3(0, 0, -0.1));
  }
}

} // namespace StarBear
