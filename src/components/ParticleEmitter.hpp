#ifndef PARTICLEEMITTER_HPP
#define PARTICLEEMITTER_HPP

#include <array>

#include "Vec3.hpp"

namespace StarBear {

struct ParticleEmitter
{
  float mRadius { 15 };

  std::array<Vec3, 500> mParticles;
  size_t mSize { 0 };
};

} // namespace StarBear

#endif
