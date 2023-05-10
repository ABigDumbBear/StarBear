#ifndef SHIPCONTROLLERSYSTEM_HPP
#define SHIPCONTROLLERSYSTEM_HPP

#include "ECS.hpp"
#include "Input.hpp"

#include "Vec3.hpp"

namespace StarBear {

class ShipControllerSystem : public System
{
  public:
    void Update(Scene& aScene, const Input& aInput, double dt);

  private:
    Vec3 mTargetPos { 0, 0, -10 };

    double mTimer { 0 };
};

} // namespace StarBear

#endif
