#ifndef PHYSICSSYSTEM_HPP
#define PHYSICSSYSTEM_HPP

#include <KumaECS/ECS.hpp>

namespace StarBear {

class PhysicsSystem : public KumaECS::System {
public:
  void Update(KumaECS::Scene &aScene, double dt);
};

} // namespace StarBear

#endif
