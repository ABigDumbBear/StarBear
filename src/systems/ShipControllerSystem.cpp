#include "ShipControllerSystem.hpp"

#include <GLFW/glfw3.h>

#include "Transform.hpp"

namespace StarBear {

/******************************************************************************/
void ShipControllerSystem::Update(Scene& aScene, const Input& aInput)
{
  for(const auto& entity : mEntities)
  {
    auto& transform = aScene.GetComponentForEntity<Transform>(entity);
    if(aInput.mPressedKeys.count(GLFW_KEY_W))
    {
      transform.Translate(Vec3(0, 1, 0));
      transform.Rotate(1, 0, 0);
    }
    if(aInput.mPressedKeys.count(GLFW_KEY_A))
    {
      transform.Translate(Vec3(-1, 0, 0));
      transform.Rotate(0, 0, -1);
    }
    if(aInput.mPressedKeys.count(GLFW_KEY_S))
    {
      transform.Translate(Vec3(0, -1, 0));
      transform.Rotate(-1, 0, 0);
    }
    if(aInput.mPressedKeys.count(GLFW_KEY_D))
    {
      transform.Translate(Vec3(1, 0, 0));
      transform.Rotate(0, 0, 1);
    }
  }
}

} // namespace StarBear
