#include "ParticleEmitterSystem.hpp"

#include "Transform.hpp"
#include "ParticleEmitter.hpp"

#include "MathUtil.hpp"

namespace StarBear {

/******************************************************************************/
void ParticleEmitterSystem::Update(Scene& aScene, std::random_device& aDevice, double dt)
{
  mTimer += dt;

  // spawn particles
  for(const auto& entity : mEntities)
  {
    auto& emitter = aScene.GetComponentForEntity<ParticleEmitter>(entity);
    auto& transform = aScene.GetComponentForEntity<Transform>(entity);

    int numParticles = emitter.mIntensity * mTimer;
    int maxParticles = emitter.mParticles.size() - emitter.mActiveParticles;
    numParticles = std::min(maxParticles, numParticles);

    for(int i = 0; i < numParticles; ++i)
    {
      std::mt19937 generator(aDevice());
      std::uniform_real_distribution<> dist(-emitter.mRadius, emitter.mRadius);

      auto pos = transform.GetTranslationMatrix() * Vec3(0, 0, 0);
      pos.x += dist(generator);
      pos.y += dist(generator);
      pos.z += dist(generator);

      auto& particle = emitter.mParticles[emitter.mActiveParticles];
      particle.mPosition = pos;
      ++emitter.mActiveParticles;
    }
  }

  // move particles
  for(const auto& entity : mEntities)
  {
    auto& emitter = aScene.GetComponentForEntity<ParticleEmitter>(entity);
    for(size_t i = 0; i < emitter.mActiveParticles; ++i)
    {
      emitter.mParticles[i].mPosition.z += 1.5;

      // if the particle is too far forward, swap it with the last
      // active particle
      if(emitter.mParticles[i].mPosition.z > 50)
      {
        --emitter.mActiveParticles;
        emitter.mParticles[i] = emitter.mParticles[emitter.mActiveParticles];
        --i;
      }
    }
  }

  if(mTimer >= 1)
  {
    mTimer = 0;
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
