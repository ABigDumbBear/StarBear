#ifndef PARENTSYSTEM_HPP
#define PARENTSYSTEM_HPP

#include <KumaECS/ECS.hpp>

namespace StarBear {

class ParentSystem : public KumaECS::System {
public:
  void Update(KumaECS::Scene &aScene);
};

} // namespace StarBear

#endif
