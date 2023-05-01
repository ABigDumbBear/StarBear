#ifndef GAME_HPP
#define GAME_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>

#include "Model.hpp"
#include "Shader.hpp"

#include "ECS.hpp"

#include "ShipControllerSystem.hpp"

namespace StarBear {

class Game
{
  public:
    Game(GLFWwindow* aWindow);

    void Run();

  private:
    GLFWwindow* mWindow;

    Shader mShader;
    Model mModel;

    Scene mScene;

    ShipControllerSystem* mShipControllerSystem;
};

} // namespace StarBear

#endif
