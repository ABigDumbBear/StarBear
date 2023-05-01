#ifndef SHIPCONTROLLERSYSTEM_HPP
#define SHIPCONTROLLERSYSTEM_HPP

#include "ECS.hpp"

namespace StarBear {

class ShipControllerSystem : public System
{
  public:
    void Update(Scene& aScene);
};

} // namespace StarBear

#endif
