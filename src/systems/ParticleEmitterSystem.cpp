#include "ParticleEmitterSystem.hpp"

#include "Transform.hpp"
#include "ParticleEmitter.hpp"

#include "MathUtil.hpp"

namespace StarBear {

/******************************************************************************/
void ParticleEmitterSystem::Update(Scene& aScene, std::random_device& aDevice, double dt)
{
  for(const auto& entity : mEntities)
  {
    auto& emitter = aScene.GetComponentForEntity<ParticleEmitter>(entity);
    auto& transform = aScene.GetComponentForEntity<Transform>(entity);

    emitter.mTimeSinceEmission += dt;

    // Determine how many particles to spawn.
    int numParticles = emitter.mIntensity * emitter.mTimeSinceEmission;
    int maxParticles = emitter.mParticles.size() - emitter.mActiveParticles;
    numParticles = std::min(maxParticles, numParticles);

    //if(numParticles >= 1) { emitter.mTimeSinceEmission = 0; }

    // Spawn particles and place them according to the radius of the emitter.
    for(int i = 0; i < numParticles; ++i)
    {
      std::mt19937 generator(aDevice());
      std::uniform_real_distribution<> dist(-emitter.mRadius, emitter.mRadius);

      auto pos = transform.GetMatrix() * transform.GetPosition();
      pos.x += dist(generator);
      pos.y += dist(generator);
      pos.z += dist(generator);

      auto& particle = emitter.mParticles[emitter.mActiveParticles];
      particle.mPosition = pos;
      ++emitter.mActiveParticles;
    }

    // For each active particle, move it according to its velocity.
    for(size_t i = 0; i < emitter.mActiveParticles; ++i)
    {
      emitter.mParticles[i].mPosition.z += 0.1;

      // Update the particle's lifetime, then if it has reached 0, swap
      // it with the last active particle in the emitter.
      emitter.mParticles[i].mLifetime -= dt;
      if(emitter.mParticles[i].mLifetime <= 0)
      {
        --emitter.mActiveParticles;
        emitter.mParticles[i] = emitter.mParticles[emitter.mActiveParticles];
        --i;
      }
    }
  }
}

/******************************************************************************/
void ParticleEmitterSystem::Render(Scene& aScene,
                                   ResourceMap& aMap,
                                   const Mat4& aView,
                                   const Mat4& aProj)
{
  std::vector<Mat4> modelMatrices;
  for(const auto& entity : mEntities)
  {
    auto& emitter = aScene.GetComponentForEntity<ParticleEmitter>(entity);
    for(size_t i = 0; i < emitter.mActiveParticles; ++i)
    {
      auto mat = Translate(emitter.mParticles[i].mPosition);
      modelMatrices.emplace_back(mat);
    }
  }

  auto& mesh = aMap.GetMesh(MeshType::eQUAD);
  auto& shader = aMap.GetShader(ShaderType::ePARTICLE);

  glBindBuffer(GL_ARRAY_BUFFER, mesh.GetInstanceBufferID());
  glBufferData(GL_ARRAY_BUFFER,
               modelMatrices.size() * sizeof(Mat4),
               modelMatrices.data(),
               GL_DYNAMIC_DRAW);

  shader.Use();
  shader.SetMat4("viewMatrix", aView);
  shader.SetMat4("projectionMatrix", aProj);

  mesh.DrawInstanced(shader, modelMatrices.size());
}

} // namespace StarBear
