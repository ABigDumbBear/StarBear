#ifndef PARTICLEEMITTER_HPP
#define PARTICLEEMITTER_HPP

#include "Vec3.hpp"

namespace StarBear {

struct ParticleEmitter
{
  float mRadius { 15 };

  Vec3 mParticles[500];
  size_t mSize { 0 };
};

} // namespace StarBear

#endif
