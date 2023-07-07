#ifndef PARTICLEEMITTERSYSTEM_HPP
#define PARTICLEEMITTERSYSTEM_HPP

#include <random>

#include <KumaECS/ECS.hpp>

#include "ResourceMap.hpp"

namespace StarBear {

class ParticleEmitterSystem : public KumaECS::System {
public:
  void Update(KumaECS::Scene &aScene, std::random_device &aDevice, double dt);
  void Render(KumaECS::Scene &aScene, ResourceMap &aMap,
              const KumaGL::Mat4 &aView, const KumaGL::Mat4 &aProj);
};

} // namespace StarBear

#endif
