#ifndef GAME_HPP
#define GAME_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <set>

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

    std::set<int> mPressedKeys;

  private:
    GLFWwindow* mWindow;

    Model mModel;
    Shader mShader;

    Scene mScene;

    ShipControllerSystem* mShipControllerSystem;
};

} // namespace StarBear

#endif
