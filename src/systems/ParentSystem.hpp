#ifndef PARENTSYSTEM_HPP
#define PARENTSYSTEM_HPP

#include "ECS.hpp"

namespace StarBear {

class ParentSystem : public System
{
  public:
    void Update(Scene& aScene);
};

} // namespace StarBear

#endif
