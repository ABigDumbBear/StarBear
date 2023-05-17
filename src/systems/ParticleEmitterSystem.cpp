#include "ParticleEmitterSystem.hpp"

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

    int numParticles = emitter.mIntensity * mTimer;
    int maxParticles = emitter.mParticles.size() - emitter.mActiveParticles;
    numParticles = std::min(maxParticles, numParticles);

    for(int i = 0; i < numParticles; ++i)
    {
      std::mt19937 generator(aDevice());
      std::uniform_real_distribution<> dist(-emitter.mRadius, emitter.mRadius);

      auto& particle = emitter.mParticles[emitter.mActiveParticles];
      particle.mPosition = Vec3(dist(generator), dist(generator), dist(generator));
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
void ParticleEmitterSystem::Render(Scene& aScene, ResourceMap& aMap)
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
  shader.SetMat4("viewMatrix", View(Vec3(0, 0, 1), Vec3(1, 0, 0), Vec3(0, 0, 50)));
  shader.SetMat4("projectionMatrix", Perspective(45, 1280, 720, 0.1, 100));

  mesh.DrawInstanced(shader, modelMatrices.size());
}

} // namespace StarBear
