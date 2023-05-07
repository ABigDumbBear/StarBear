#ifndef LASERSYSTEM_HPP
#define LASERSYSTEM_HPP

#include "ECS.hpp"

#include "Mesh.hpp"
#include "Shader.hpp"

namespace StarBear {

class LaserSystem : public System
{
  public:
    LaserSystem();

    void Update(Scene& aScene);
    void Render(Scene& aScene);

  private:
    Mesh mMesh;
    Shader mShader;
};

} // namespace StarBear

#endif
