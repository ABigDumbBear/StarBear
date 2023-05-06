#ifndef GAME_HPP
#define GAME_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ECS.hpp"
#include "Input.hpp"

#include "PhysicsSystem.hpp"
#include "ShipControllerSystem.hpp"
#include "ShipRenderSystem.hpp"

namespace StarBear {

class Game
{
  public:
    Game(GLFWwindow* aWindow);

    void Run();

    void FramebufferSizeCallback(int aWidth, int aHeight);
    void KeyCallback(int aKey, int aScancode, int aAction, int aMods);

  private:
    GLFWwindow* mWindow;
    Input mInput;

    Scene mScene;

    double mLastFrameTime;

    ShipControllerSystem* mShipControllerSystem;
    PhysicsSystem* mPhysicsSystem;
    ShipRenderSystem* mShipRenderSystem;
};

} // namespace StarBear

#endif
