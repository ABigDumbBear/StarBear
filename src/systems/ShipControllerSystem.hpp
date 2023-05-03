#ifndef SHIPCONTROLLERSYSTEM_HPP
#define SHIPCONTROLLERSYSTEM_HPP

#include "ECS.hpp"
#include "Input.hpp"

namespace StarBear {

class ShipControllerSystem : public System
{
  public:
    void Update(Scene& aScene, const Input& aInput);
};

} // namespace StarBear

#endif
