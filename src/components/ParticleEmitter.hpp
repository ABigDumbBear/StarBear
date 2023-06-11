#ifndef PARTICLEEMITTER_HPP
#define PARTICLEEMITTER_HPP

#include <array>

#include "Vec3.hpp"

namespace StarBear {

struct Particle
{
  float mMaxLifetime { 2 };
  float mLifetime { 2 };

  Vec3 mPosition;
  Vec3 mVelocity;
};

struct ParticleEmitter
{
  Vec3 mPreviousPosition;

  float mEmissionRate { 1 };

  float mRadius { 15 };

  size_t mActiveParticles { 0 };
  std::array<Particle, 5000> mParticles;
};

} // namespace StarBear

#endif
