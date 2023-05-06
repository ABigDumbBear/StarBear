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

    void Update(Scene& aScene, std::random_device& aDevice);
    void Render(Scene& aScene);

  private:
    size_t mFrameCounter { 0 };

    Mesh mMesh;
    Shader mShader;
};

} // namespace StarBear

#endif
