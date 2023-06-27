#ifndef RAILMOVERSYSTEM_HPP
#define RAILMOVERSYSTEM_HPP

#include "ECS.hpp"

namespace StarBear {

class RailMoverSystem : public System {
public:
  void Update(Scene &aScene);
};

} // namespace StarBear

#endif
