#include "ParticleEmitterSystem.hpp"

#include "ParticleEmitter.hpp"

#include "MathUtil.hpp"

namespace StarBear {

/******************************************************************************/
ParticleEmitterSystem::ParticleEmitterSystem()
{
  MeshVertex vertex;
  vertex.mPosition = Vec3(0, 0, 0);
  mMesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(0.1, 0, 0);
  mMesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(0.1, 0.1, 0);
  mMesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(0, 0.1, 0);
  mMesh.mVertices.emplace_back(vertex);

  mMesh.mIndices.emplace_back(0);
  mMesh.mIndices.emplace_back(1);
  mMesh.mIndices.emplace_back(3);
  mMesh.mIndices.emplace_back(3);
  mMesh.mIndices.emplace_back(1);
  mMesh.mIndices.emplace_back(2);

  mMesh.UpdateVertices();
  mMesh.UpdateIndices();

  mShader.LoadFromFiles("resources/shaders/Particle.vert",
                        "resources/shaders/Particle.frag");
}

/******************************************************************************/
void ParticleEmitterSystem::Update(Scene& aScene, std::random_device& aDevice, double dt)
{
  // spawn particles
  mTimer += dt;
  if(mTimer > 0.02)
  {
    for(const auto& entity : mEntities)
    {
      auto& emitter = aScene.GetComponentForEntity<ParticleEmitter>(entity);
      if(emitter.mSize < emitter.mParticles.size())
      {
        std::mt19937 generator(aDevice());
        std::uniform_real_distribution<> dist(-emitter.mRadius, emitter.mRadius);

        emitter.mParticles[emitter.mSize] = Vec3(dist(generator), dist(generator), -30);
        ++emitter.mSize;
      }
    }

    mTimer = 0;
  }

  // move particles
  for(const auto& entity : mEntities)
  {
    auto& emitter = aScene.GetComponentForEntity<ParticleEmitter>(entity);
    for(size_t i = 0; i < emitter.mSize; ++i)
    {
      emitter.mParticles[i].z += 1.5;

      // if the particle is too far forward, swap it with the last
      // active particle
      if(emitter.mParticles[i].z > 50)
      {
        --emitter.mSize;
        emitter.mParticles[i] = emitter.mParticles[emitter.mSize];
        --i;
      }
    }
  }
}

/******************************************************************************/
void ParticleEmitterSystem::Render(Scene& aScene)
{
  std::vector<Mat4> modelMatrices;
  for(const auto& entity : mEntities)
  {
    auto& emitter = aScene.GetComponentForEntity<ParticleEmitter>(entity);
    for(size_t i = 0; i < emitter.mSize; ++i)
    {
      auto mat = Translate(emitter.mParticles[i]);
      modelMatrices.emplace_back(mat);
    }
  }

  glBindBuffer(GL_ARRAY_BUFFER, mMesh.GetInstanceBufferID());
  glBufferData(GL_ARRAY_BUFFER,
               modelMatrices.size() * sizeof(Mat4),
               modelMatrices.data(),
               GL_DYNAMIC_DRAW);

  mShader.Use();
  mShader.SetMat4("viewMatrix", View(Vec3(0, 0, 1), Vec3(1, 0, 0), Vec3(0, 0, 50)));
  mShader.SetMat4("projectionMatrix", Perspective(45, 1280, 720, 0.1, 100));

  mMesh.DrawInstanced(mShader, modelMatrices.size());
}

} // namespace StarBear
