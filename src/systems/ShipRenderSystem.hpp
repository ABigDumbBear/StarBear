#ifndef SHIPRENDERSYSTEM_HPP
#define SHIPRENDERSYSTEM_HPP

#include "ECS.hpp"
#include "ResourceMap.hpp"

namespace StarBear {

class ShipRenderSystem : public System
{
  public:
    void Render(Scene& aScene, ResourceMap& aMap);
};

} // namespace StarBear

#endif
