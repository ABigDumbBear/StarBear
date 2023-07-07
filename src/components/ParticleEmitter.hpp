#ifndef PARTICLEEMITTER_HPP
#define PARTICLEEMITTER_HPP

#include <array>

#include <KumaGL/Vec3.hpp>

namespace StarBear {

struct Particle {
  float mMaxLifetime{0.2};
  float mLifetime{0.2};

  KumaGL::Vec3 mPosition;
  KumaGL::Vec3 mVelocity;
};

struct ParticleEmitter {
  KumaGL::Vec3 mPreviousPosition;

  float mEmissionRate{1};

  float mRadius{15};

  size_t mActiveParticles{0};
  std::array<Particle, 5000> mParticles;
};

} // namespace StarBear

#endif
