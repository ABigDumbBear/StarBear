#ifndef GAME_HPP
#define GAME_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Model.hpp"
#include "Shader.hpp"

#include "ECS.hpp"
#include "Input.hpp"

#include "ShipControllerSystem.hpp"

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

    Model mModel;
    Shader mShader;

    Scene mScene;

    ShipControllerSystem* mShipControllerSystem;
};

} // namespace StarBear

#endif
