#ifndef PHYSICS_HPP
#define PHYSICS_HPP

#include <KumaGL/Vec3.hpp>

namespace StarBear {

struct Physics {
  KumaGL::Vec3 mAcceleration;
  KumaGL::Vec3 mVelocity;
};

} // namespace StarBear

#endif
