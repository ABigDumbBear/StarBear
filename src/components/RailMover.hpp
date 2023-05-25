#ifndef RAILMOVER_HPP
#define RAILMOVER_HPP

#include <set>

#include "ECS.hpp"

namespace StarBear {

struct RailMover
{
  std::set<Entity> mChildren;
};

} // namespace StarBear

#endif
