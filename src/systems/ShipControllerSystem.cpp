#include "ShipControllerSystem.hpp"

#include <GLFW/glfw3.h>

#include "MathUtil.hpp"

#include "Laser.hpp"
#include "ShipController.hpp"
#include "Transform.hpp"
#include "Hitbox.hpp"

namespace StarBear {

/******************************************************************************/
void ShipControllerSystem::Update(Scene& aScene, const Input& aInput, double dt)
{
  mTimer += dt;

  if(aInput.mPressedKeys.count(GLFW_KEY_W))
  {
    mTargetPos.y += 1;
  }
  if(aInput.mPressedKeys.count(GLFW_KEY_A))
  {
    mTargetPos.x -= 1;
  }
  if(aInput.mPressedKeys.count(GLFW_KEY_S))
  {
    mTargetPos.y -= 1;
  }
  if(aInput.mPressedKeys.count(GLFW_KEY_D))
  {
    mTargetPos.x += 1;
  }

  if(aInput.mPressedKeys.count(GLFW_KEY_SPACE) && mTimer > 1)
  {
    // create a laser
    auto laser = aScene.CreateEntity();
    aScene.AddComponentToEntity<Laser>(laser);
    aScene.AddComponentToEntity<Transform>(laser);
    aScene.AddComponentToEntity<Hitbox>(laser);

    aScene.GetComponentForEntity<Transform>(laser).SetPosition(mTargetPos);

    mTimer = 0;
  }

  for(const auto& entity : mEntities)
  {
    auto& controller = aScene.GetComponentForEntity<ShipController>(entity);
    auto& transform = aScene.GetComponentForEntity<Transform>(entity);

    auto newPos = transform.GetPosition();
    newPos = Lerp(newPos, mTargetPos, 0.3);
    newPos.z = 0;
    transform.SetPosition(newPos);
  }
}

} // namespace StarBear
