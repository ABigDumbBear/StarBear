#ifndef PARTICLEEMITTERSYSTEM_HPP
#define PARTICLEEMITTERSYSTEM_HPP

#include "ECS.hpp"

#include <random>

#include "Mesh.hpp"
#include "Shader.hpp"

namespace StarBear {

class ParticleEmitterSystem : public System
{
  public:
    ParticleEmitterSystem();

    void Update(Scene& aScene, std::random_device& aDevice, double dt);
    void Render(Scene& aScene);

  private:
    Mesh mMesh;
    Shader mShader;

    double mTimer { 0 };
};

} // namespace StarBear

#endif
