#ifndef PARTICLEEMITTERSYSTEM_HPP
#define PARTICLEEMITTERSYSTEM_HPP

#include "ECS.hpp"
#include "ResourceMap.hpp"

#include <random>

namespace StarBear {

class ParticleEmitterSystem : public System
{
  public:
    void Update(Scene& aScene, std::random_device& aDevice, double dt);
    void Render(Scene& aScene, ResourceMap& aMap);

  private:
    double mTimer { 0 };
};

} // namespace StarBear

#endif
