#ifndef RAILMOVERSYSTEM_HPP
#define RAILMOVERSYSTEM_HPP

#include <KumaECS/ECS.hpp>

namespace StarBear {

class RailMoverSystem : public KumaECS::System {
public:
  void Update(KumaECS::Scene &aScene);
};

} // namespace StarBear

#endif
