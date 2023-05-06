#include "ShipControllerSystem.hpp"

#include <GLFW/glfw3.h>

#include "MathUtil.hpp"

#include "ShipController.hpp"
#include "Transform.hpp"

namespace StarBear {

/******************************************************************************/
void ShipControllerSystem::Update(Scene& aScene, const Input& aInput)
{
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

  for(const auto& entity : mEntities)
  {
    auto& controller = aScene.GetComponentForEntity<ShipController>(entity);
    auto& transform = aScene.GetComponentForEntity<Transform>(entity);

    auto newPos = transform.GetPosition();
    newPos = Lerp(newPos, mTargetPos, 0.1);
    newPos.z = 0;
    transform.SetPosition(newPos);
  }
}

} // namespace StarBear
