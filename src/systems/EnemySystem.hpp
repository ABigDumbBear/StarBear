#ifndef ENEMYSYSTEM_HPP
#define ENEMYSYSTEM_HPP

#include "ECS.hpp"
#include "ResourceMap.hpp"

namespace StarBear {

class EnemySystem : public System
{
  public:
    void Update(Scene& aScene, double dt);
    void Render(Scene& aScene,
                ResourceMap& aMap,
                const Mat4& aView,
                const Mat4& aProj);

  private:
    double mTimer { 0 };
};

} // namespace StarBear

#endif
