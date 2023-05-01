#include "Game.hpp"

#include <iostream>

#include "MathUtil.hpp"
#include "Transform.hpp"

namespace StarBear {

/******************************************************************************/
void GLFWErrorCallback(int aError, const char* aDescription)
{
  std::cout << "GLFW encountered an error: " << aDescription << std::endl;
}

/******************************************************************************/
void GLFWFramebufferSizeCallback(GLFWwindow* aWindow, int aWidth, int aHeight)
{
  glViewport(0, 0, aWidth, aHeight);
}

/******************************************************************************/
Game::Game(GLFWwindow* aWindow)
  : mWindow(aWindow)
  , mShipControllerSystem(nullptr)
{
  // Set GLFW callbacks.
  glfwSetFramebufferSizeCallback(mWindow, GLFWFramebufferSizeCallback);
  glfwSetErrorCallback(GLFWErrorCallback);

  // Load resources.
  mShader.LoadFromFiles("resources/shaders/Ship.vert",
                        "resources/shaders/Ship.frag");
  mModel.LoadFromFile("resources/models/Spitfire/OBJ/Spitfire.obj");

  // Set up the scene.
  mScene.RegisterComponentType<Transform>(1);

  Signature sig;
  mScene.AddComponentToSignature<Transform>(sig);
  mShipControllerSystem = mScene.RegisterSystemType<ShipControllerSystem>(sig);

  auto ship = mScene.CreateEntity();
  mScene.AddComponentToEntity<Transform>(ship);
}

/******************************************************************************/
void Game::Run()
{
  // Run until instructed to close.
  while(!glfwWindowShouldClose(mWindow))
  {
    glfwSwapBuffers(mWindow);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mShipControllerSystem->Update(mScene);

    auto& modelTransform = mScene.GetComponentForEntity<Transform>(0);
    mShader.Use();
    mShader.SetMat4("modelMatrix", modelTransform.GetMatrix());
    mShader.SetMat4("viewMatrix", View(Vec3(0, 0, 1), Vec3(1, 0, 0), Vec3(0, 0, 10)));
    mShader.SetMat4("projectionMatrix", Perspective(45, 1280, 720, 0.1, 100));
    mShader.SetMat4("modelMatrix", modelTransform.GetMatrix());

    mModel.Draw(mShader);

    glfwPollEvents();
  }
}

} // namespace StarBear
