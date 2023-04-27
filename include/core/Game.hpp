#ifndef GAME_HPP
#define GAME_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Model.hpp"
#include "Shader.hpp"

namespace StarBear {

class Game
{
  public:
    Game(GLFWwindow* aWindow);

    void Run();

  private:
    GLFWwindow* mWindow;

    KumaGL::Shader mShader;
    KumaGL::Model mModel;
};

} // namespace StarBear

#endif
