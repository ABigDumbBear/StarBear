#ifndef SHIPCONTROLLERSYSTEM_HPP
#define SHIPCONTROLLERSYSTEM_HPP

#include "ECS.hpp"
#include "Input.hpp"
#include "ResourceMap.hpp"

#include "Vec3.hpp"

namespace StarBear {

class ShipControllerSystem : public System
{
  public:
    void Update(Scene& aScene, const Input& aInput, double dt);
    void Render(Scene& aScene, ResourceMap& aMap);

  private:
    Vec3 mTargetPos { 0, 0, -1 };

    double mTimer { 0 };
};

} // namespace StarBear

#endif
