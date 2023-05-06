#ifndef PHYSICSSYSTEM_HPP
#define PHYSICSSYSTEM_HPP

#include "ECS.hpp"

namespace StarBear {

class PhysicsSystem : public System
{
  public:
    void Update(Scene& aScene, double dt);
};

} // namespace FoolsErrand

#endif
