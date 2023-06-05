#include "ParticleEmitterSystem.hpp"

#include "Transform.hpp"
#include "ParticleEmitter.hpp"

#include "MathUtil.hpp"

#include <iostream>

namespace StarBear {

/******************************************************************************/
void ParticleEmitterSystem::Update(Scene& aScene, std::random_device& aDevice, double dt)
{
  for(const auto& entity : mEntities)
  {
    auto& emitter = aScene.GetComponentForEntity<ParticleEmitter>(entity);
    auto& transform = aScene.GetComponentForEntity<Transform>(entity);

    // Determine the current position of the emitter.
    std::cout << "emitter matrix: " << std::endl << transform.GetMatrix() << std::endl;
    auto emitterPos = transform.GetMatrix() * Vec3(0, 0, 0);
    std::cout << "emitterPos: " << emitterPos << std::endl;
    
    //std::cout << "emitter local position: " << transform.GetPosition() << std::endl;
    //std::cout << "emitter global position: " << transform.GetMatrix() * Vec3(0, 0, 0) << std::endl;
    //std::cout << "translation matrix: " << std::endl << transform.GetTranslationMatrix() << std::endl;
    //std::cout << "rotation matrix: " << std::endl << transform.GetRotationMatrix() << std::endl;

    // Determine the number of particles to spawn.
    size_t numParticles = emitter.mEmissionRate * dt;
    size_t availableParticles = emitter.mParticles.size() - emitter.mActiveParticles;
    numParticles = std::min(numParticles, availableParticles);

    for(size_t i = 0; i < numParticles; ++i)
    {
      std::mt19937 generator(aDevice());
      std::uniform_real_distribution<> dist(-emitter.mRadius, emitter.mRadius);

      // Generate a random position for the new particle based on the emitter's
      // current position.
      auto particlePos = emitterPos;
      particlePos.x += dist(generator);
      particlePos.y += dist(generator);
      particlePos.z += dist(generator);

      // Interpolate between the emitter's previous position and the particle's
      // position for the appearance of smooth emission.
      auto& particle = emitter.mParticles[emitter.mActiveParticles];
      particle.mPosition = Lerp(emitter.mPreviousPosition, particlePos, ((float)i / (float)numParticles));

      ++emitter.mActiveParticles;
    }

    // Store the previous position of the emitter for interpolation.
    emitter.mPreviousPosition = emitterPos;

    // For each active particle, move it according to its velocity.
    for(size_t i = emitter.mActiveParticles; i > 0; --i)
    {
      emitter.mParticles[i - 1].mPosition += (transform.GetForward() * -1) * 0.1;

      // Update the particle's lifetime, then if it has reached 0, swap
      // it with the last active particle in the emitter.
      emitter.mParticles[i - 1].mLifetime -= dt;
      if(emitter.mParticles[i - 1].mLifetime <= 0)
      {
        auto lastValidIndex = emitter.mActiveParticles - 1;

        emitter.mParticles[i - 1] = emitter.mParticles[lastValidIndex];
        emitter.mParticles[lastValidIndex] = Particle();

        --emitter.mActiveParticles;
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
  std::vector<float> lifetimes;
  for(const auto& entity : mEntities)
  {
    auto& emitter = aScene.GetComponentForEntity<ParticleEmitter>(entity);
    for(size_t i = 0; i < emitter.mActiveParticles; ++i)
    {
      auto mat = Translate(emitter.mParticles[i].mPosition);
      mat = mat * Scale(Vec3((emitter.mParticles[i].mLifetime / emitter.mParticles[i].mMaxLifetime),
                             (emitter.mParticles[i].mLifetime / emitter.mParticles[i].mMaxLifetime),
                             1));
      modelMatrices.emplace_back(mat);

      lifetimes.emplace_back(emitter.mParticles[i].mLifetime / emitter.mParticles[i].mMaxLifetime);
    }
  }

  auto& mesh = aMap.GetMesh(MeshType::eQUAD);
  auto& shader = aMap.GetShader(ShaderType::ePARTICLE);
  auto& texture = aMap.GetTexture(TextureType::ePARTICLE);

  glBindBuffer(GL_ARRAY_BUFFER, mesh.GetInstanceBufferID());
  glBufferData(GL_ARRAY_BUFFER,
               modelMatrices.size() * sizeof(Mat4),
               modelMatrices.data(),
               GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, mesh.GetCustomBufferID());
  glBufferData(GL_ARRAY_BUFFER,
               lifetimes.size() * sizeof(float),
               lifetimes.data(),
               GL_DYNAMIC_DRAW);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture.GetID());

  shader.Use();
  shader.SetMat4("viewMatrix", aView);
  shader.SetMat4("projectionMatrix", aProj);

  mesh.DrawInstanced(shader, modelMatrices.size());
}

} // namespace StarBear
