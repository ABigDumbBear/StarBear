#ifndef SHIPCONTROLLER_HPP
#define SHIPCONTROLLER_HPP

#include "Vec3.hpp"

namespace StarBear {

struct ShipController
{
  Vec3 mForward { 0, 0, -1 };
  float mSpeed { 0 };
};

} // namespace StarBear

#endif
