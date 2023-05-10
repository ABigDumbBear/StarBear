#ifndef ENEMYSYSTEM_HPP
#define ENEMYSYSTEM_HPP

#include "ECS.hpp"

#include "Model.hpp"
#include "Shader.hpp"

namespace StarBear {

class EnemySystem : public System
{
  public:
    EnemySystem();

    void Update(Scene& aScene, double dt);
    void Render(Scene& aScene);

  private:
    double mTimer { 0 };

    Model mModel;
    Shader mShader;
};

} // namespace StarBear

#endif
