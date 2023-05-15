#ifndef COLLISIONSYSTEM_HPP
#define COLLISIONSYSTEM_HPP

#include "ECS.hpp"

#include "Hitbox.hpp"
#include "Transform.hpp"

#include "Mesh.hpp"
#include "Shader.hpp"

namespace StarBear {

class CollisionSystem : public System
{
  public:
    CollisionSystem();

    void Update(Scene& aScene);
    void Render(Scene& aScene);

  private:
    bool CheckCollision(const Transform& aTransformA,
                        const Hitbox& aHitboxA,
                        const Transform& aTransformB,
                        const Hitbox& aHitboxB);

    Mesh mMesh;
    Shader mShader;
};

} // namespace StarBear

#endif
