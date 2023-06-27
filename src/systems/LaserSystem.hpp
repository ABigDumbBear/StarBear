#ifndef LASERSYSTEM_HPP
#define LASERSYSTEM_HPP

#include "ECS.hpp"
#include "ResourceMap.hpp"

namespace StarBear {

class LaserSystem : public System {
public:
  void Update(Scene &aScene, double dt);
  void Render(Scene &aScene, ResourceMap &aMap, const Mat4 &aView,
              const Mat4 &aProj);
};

} // namespace StarBear

#endif
