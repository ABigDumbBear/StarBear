#include "CollisionSystem.hpp"

#include "Mat4.hpp"

#include <iostream>

namespace StarBear {

/******************************************************************************/
CollisionSystem::CollisionSystem()
{
  MeshVertex vertex;
  vertex.mPosition = Vec3(-0.5, 0, 0);
  mMesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(0.5, 0, 0);
  mMesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(0.5, 0, 3.5);
  mMesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(-0.5, 0, 3.5);
  mMesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(-0.5, 0.5, 0);
  mMesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(0.5, 0.5, 0);
  mMesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(0.5, 0.5, 3.5);
  mMesh.mVertices.emplace_back(vertex);
  vertex.mPosition = Vec3(-0.5, 0.5, 3.5);
  mMesh.mVertices.emplace_back(vertex);

  mMesh.mIndices.emplace_back(0);
  mMesh.mIndices.emplace_back(1);
  mMesh.mIndices.emplace_back(3);
  mMesh.mIndices.emplace_back(3);
  mMesh.mIndices.emplace_back(1);
  mMesh.mIndices.emplace_back(2);
  mMesh.mIndices.emplace_back(4);
  mMesh.mIndices.emplace_back(5);
  mMesh.mIndices.emplace_back(7);
  mMesh.mIndices.emplace_back(7);
  mMesh.mIndices.emplace_back(5);
  mMesh.mIndices.emplace_back(6);

  mMesh.UpdateVertices();
  mMesh.UpdateIndices();

  mShader.LoadFromFiles("resources/shaders/Hitbox.vert",
                        "resources/shaders/Hitbox.frag");
}

/******************************************************************************/
void CollisionSystem::Update(Scene& aScene)
{
  for(const auto& entity : mEntities)
  {
    auto& hitbox = aScene.GetComponentForEntity<Hitbox>(entity);
    hitbox.mCollided = false;
  }

  for(const auto& entityA : mEntities)
  {
    for(const auto& entityB : mEntities)
    {
      if(entityA == entityB) { continue; }

      auto& transformA = aScene.GetComponentForEntity<Transform>(entityA);
      auto& hitboxA = aScene.GetComponentForEntity<Hitbox>(entityA);
      auto& transformB = aScene.GetComponentForEntity<Transform>(entityB);
      auto& hitboxB = aScene.GetComponentForEntity<Hitbox>(entityB);

      if(!hitboxB.mCollided)
      {
        hitboxB.mCollided = CheckCollision(transformA, hitboxA, transformB, hitboxB);
      }
    }
  }
}

/******************************************************************************/
void CollisionSystem::Render(Scene& aScene)
{
  std::vector<Mat4> modelMatrices;
  for(const auto& entity : mEntities)
  {
    auto& transform = aScene.GetComponentForEntity<Transform>(entity);
    auto& hitbox = aScene.GetComponentForEntity<Hitbox>(entity);

    auto mat = Translate(transform.GetPosition());
    mat = mat * Scale(Vec3(hitbox.mWidth, hitbox.mHeight, hitbox.mDepth));
    modelMatrices.emplace_back(mat);
  }

  glBindBuffer(GL_ARRAY_BUFFER, mMesh.GetInstanceBufferID());
  glBufferData(GL_ARRAY_BUFFER,
               modelMatrices.size() * sizeof(Mat4),
               modelMatrices.data(),
               GL_DYNAMIC_DRAW);

  mShader.Use();
  mShader.SetMat4("viewMatrix", View(Vec3(0, 0, 1), Vec3(1, 0, 0), Vec3(0, 0, 50)));
  mShader.SetMat4("projectionMatrix", Perspective(45, 1280, 720, 0.1, 1000));

  mMesh.DrawInstanced(mShader, modelMatrices.size(), GL_LINE_LOOP);
}

/******************************************************************************/
bool CollisionSystem::CheckCollision(const Transform& aTransformA,
                                     const Hitbox& aHitboxA,
                                     const Transform& aTransformB,
                                     const Hitbox& aHitboxB)
{
  bool collision = false;

  auto leftA = aTransformA.GetPosition().x + aHitboxA.x;
  auto rightA = leftA + aHitboxA.mWidth;
  auto bottomA = aTransformA.GetPosition().y + aHitboxA.y;
  auto topA = bottomA + aHitboxA.mHeight;
  auto farA = aTransformA.GetPosition().z + aHitboxA.z;
  auto nearA = farA + aHitboxA.mDepth;

  auto leftB = aTransformB.GetPosition().x + aHitboxB.x;
  auto rightB = leftB + aHitboxB.mWidth;
  auto bottomB = aTransformB.GetPosition().y + aHitboxB.y;
  auto topB = bottomB + aHitboxB.mHeight;
  auto farB = aTransformB.GetPosition().z + aHitboxB.z;
  auto nearB = farB + aHitboxB.mDepth;

  if(rightA >= leftB && rightB >= leftA)
  {
    if(topA >= bottomB && topB >= bottomA)
    {
      if(nearA >= farB && nearB >= farA)
      {
        std::cout << "---------------------- collision!" << std::endl;
        std::cout << "leftA: " << leftA << std::endl;
        std::cout << "rightA: " << rightA << std::endl;
        std::cout << "bottomA: " << bottomA << std::endl;
        std::cout << "topA: " << topA << std::endl;
        std::cout << "farA: " << farA << std::endl;
        std::cout << "nearA: " << nearA << std::endl;
        std::cout << std::endl;

        std::cout << "leftB: " << leftB << std::endl;
        std::cout << "rightB: " << rightB << std::endl;
        std::cout << "bottomB: " << bottomB << std::endl;
        std::cout << "topB: " << topB << std::endl;
        std::cout << "farB: " << farB << std::endl;
        std::cout << "nearB: " << nearB << std::endl;

        collision = true;
      }
    }
  }

  return collision;
}

} // namespace StarBear
