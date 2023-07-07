#ifndef SHIPCONTROLLERSYSTEM_HPP
#define SHIPCONTROLLERSYSTEM_HPP

#include <KumaECS/ECS.hpp>

#include <KumaGL/Mat4.hpp>
#include <KumaGL/Vec3.hpp>

#include "Input.hpp"
#include "ResourceMap.hpp"

namespace StarBear {

class ShipControllerSystem : public KumaECS::System {
public:
  void Update(KumaECS::Scene &aScene, const Input &aInput, double dt);
  void Render(KumaECS::Scene &aScene, ResourceMap &aMap,
              const KumaGL::Mat4 &aView, const KumaGL::Mat4 &aProj);
};

} // namespace StarBear

#endif
