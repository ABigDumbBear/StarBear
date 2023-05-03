#include "Game.hpp"

#include <iostream>

#include "MathUtil.hpp"
#include "Transform.hpp"

#include "ShipController.hpp"

namespace StarBear {

/******************************************************************************/
Game::Game(GLFWwindow* aWindow)
  : mWindow(aWindow)
  , mShipControllerSystem(nullptr)
{
  // Set GLFW callbacks.
  glfwSetWindowUserPointer(mWindow, this);
  glfwSetErrorCallback([](int aError, const char* aDescription)
  {
    std::cout << "GLFW encountered an error: " << aDescription << std::endl;
  });

  glfwSetFramebufferSizeCallback(mWindow, [](GLFWwindow* aWindow, int aWidth, int aHeight)
  {
    auto game = static_cast<Game*>(glfwGetWindowUserPointer(aWindow));
    game->FramebufferSizeCallback(aWidth, aHeight);
  });

  glfwSetKeyCallback(mWindow, [](GLFWwindow* aWindow, int aKey, int aScancode, int aAction, int aMods)
  {
    auto game = static_cast<Game*>(glfwGetWindowUserPointer(aWindow));
    game->KeyCallback(aKey, aScancode, aAction, aMods);
  });

  // Load resources.
  mShader.LoadFromFiles("resources/shaders/Ship.vert",
                        "resources/shaders/Ship.frag");
  mModel.LoadFromFile("resources/models/Spitfire/OBJ/Spitfire.obj");

  // Register components.
  mScene.RegisterComponentType<Transform>(1);
  mScene.RegisterComponentType<ShipController>(1);

  // Register systems.
  Signature sig;
  mScene.AddComponentToSignature<Transform>(sig);
  mScene.AddComponentToSignature<ShipController>(sig);
  mShipControllerSystem = mScene.RegisterSystemType<ShipControllerSystem>(sig);

  // Create entities.
  auto ship = mScene.CreateEntity();
  mScene.AddComponentToEntity<Transform>(ship);
  mScene.GetComponentForEntity<Transform>(ship).Rotate(0, 180, 0);
  mScene.AddComponentToEntity<ShipController>(ship);
}

/******************************************************************************/
void Game::Run()
{
  // Run until instructed to close.
  while(!glfwWindowShouldClose(mWindow))
  {
    glfwSwapBuffers(mWindow);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mShipControllerSystem->Update(mScene, mInput);

    auto& modelTransform = mScene.GetComponentForEntity<Transform>(0);
    mShader.Use();
    mShader.SetMat4("modelMatrix", modelTransform.GetMatrix());
    mShader.SetMat4("viewMatrix", View(Vec3(0, 0, 1), Vec3(1, 0, 0), Vec3(0, 0, 30)));
    mShader.SetMat4("projectionMatrix", Perspective(45, 1280, 720, 0.1, 100));
    mShader.SetMat4("modelMatrix", modelTransform.GetMatrix());

    mModel.Draw(mShader);

    glfwPollEvents();
  }
}

/******************************************************************************/
void Game::FramebufferSizeCallback(int aWidth, int aHeight)
{
  glViewport(0, 0, aWidth, aHeight);
}

/******************************************************************************/
void Game::KeyCallback(int aKey, int aScancode, int aAction, int aMods)
{
  if(aAction == GLFW_PRESS)
  {
    mInput.mPressedKeys.insert(aKey);
  }
  else if(aAction == GLFW_RELEASE)
  {
    mInput.mPressedKeys.erase(aKey);
  }
}

} // namespace StarBear
