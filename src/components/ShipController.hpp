#ifndef SHIPCONTROLLER_HPP
#define SHIPCONTROLLER_HPP

#include "Vec3.hpp"

namespace StarBear {

enum class ShipState
{
  eDEFAULT,
  eROLLING
};

struct ShipController
{
  Vec3 mTargetPos { 0, 0, -1 };
  Vec3 mForward { 0, 0, -1 };
  float mSpeed { 0 };

  ShipState mState { ShipState::eDEFAULT };

  float mRollRotation { 0 };

  float mFireRate { 10 };
  double mTimeSinceFired { 0 };
};

} // namespace StarBear

#endif
