#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

#include "Game.hpp"

/******************************************************************************/
int main() {
  // Initialize GLFW.
  if (!glfwInit()) {
    std::cout << "Failed to initialize GLFW!" << std::endl;
    return -1;
  }

  // Set the desired OpenGL version to 3.3.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  // Use the core profile only; this removes backwards-compatible features
  // that are no longer needed for the engine.
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Enable forward compatibility; this removes all deprecated features
  // in the desired version of OpenGL (3.3).
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

  // Enable double buffering.
  glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

  // Create a new window.
  auto window = glfwCreateWindow(1280, 720, "StarBear", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create window!" << std::endl;
    return -1;
  }
  glfwMakeContextCurrent(window);

  // Initialize GLAD.
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD!" << std::endl;
    return -1;
  }

  glViewport(0, 0, 1280, 720);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Create a Game and run it.
  auto game = std::make_unique<StarBear::Game>(window);
  game->Run();
  game.reset(nullptr);

  glfwTerminate();
  return 0;
}
