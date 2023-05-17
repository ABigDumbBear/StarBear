#ifndef PARTICLEEMITTER_HPP
#define PARTICLEEMITTER_HPP

#include <array>

#include "Vec3.hpp"

namespace StarBear {

struct Particle
{
  float mLifetime { 1 };
  Vec3 mVelocity;
  Vec3 mPosition;
};

struct ParticleEmitter
{
  int mIntensity { 1 };
  float mRadius { 15 };

  size_t mActiveParticles { 0 };
  std::array<Particle, 500> mParticles;
};

} // namespace StarBear

#endif
