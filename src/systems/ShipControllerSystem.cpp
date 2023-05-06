#include "ShipControllerSystem.hpp"

#include <GLFW/glfw3.h>

#include "MathUtil.hpp"

#include "Physics.hpp"
#include "ShipController.hpp"
#include "Transform.hpp"

#include <iostream>

namespace StarBear {

/******************************************************************************/
void ShipControllerSystem::Update(Scene& aScene, const Input& aInput)
{
  for(const auto& entity : mEntities)
  {
    auto& controller = aScene.GetComponentForEntity<ShipController>(entity);
    auto& transform = aScene.GetComponentForEntity<Transform>(entity);
    auto& physics = aScene.GetComponentForEntity<Physics>(entity);
    physics.mAcceleration = Vec3(0, 0, 0);

    if(aInput.mPressedKeys.count(GLFW_KEY_W))
    {
      //physics.mAcceleration.y = 5;
      mTargetPos.y += 1;
    }
    if(aInput.mPressedKeys.count(GLFW_KEY_A))
    {
      //physics.mAcceleration.x = -5;
      mTargetPos.x -= 1;
    }
    if(aInput.mPressedKeys.count(GLFW_KEY_S))
    {
      //physics.mAcceleration.y = -5;
      mTargetPos.y -= 1;
    }
    if(aInput.mPressedKeys.count(GLFW_KEY_D))
    {
      //physics.mAcceleration.x = 5;
      mTargetPos.x += 1;
    }

    transform.Rotate(Vec3(0.1, 0, 0), 10);

    /*auto dir = Normalize(controller.mForward - mTargetPos);
    auto axis = Cross(dir, controller.mForward);
    auto angle = acos(Dot(controller.mForward, dir));

    std::cout << "rotating on " << axis << " by " << angle << std::endl;
    transform.Rotate(axis, angle);
    //transform.SetRotation(axis, angle);

    controller.mForward = dir;*/
  }
}

} // namespace StarBear
