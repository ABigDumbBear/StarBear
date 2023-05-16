#include "ShipControllerSystem.hpp"

#include <GLFW/glfw3.h>

#include "EntityFactory.hpp"
#include "MathUtil.hpp"

#include "ShipController.hpp"
#include "Transform.hpp"

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

  if(aInput.mPressedKeys.count(GLFW_KEY_SPACE) && mTimer > 0.1)
  {
    // create a laser
    auto laser = CreateLaser(aScene);
    aScene.GetComponentForEntity<Transform>(laser).SetPosition(mTargetPos);

    laser = CreateLaser(aScene);
    aScene.GetComponentForEntity<Transform>(laser).SetPosition(mTargetPos + Vec3(2, 0, 0));

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
