#include "Game.hpp"

#include <iostream>

#include "MathUtil.hpp"

#include "Laser.hpp"
#include "ParticleEmitter.hpp"
#include "Physics.hpp"
#include "ShipController.hpp"
#include "Transform.hpp"

namespace StarBear {

/******************************************************************************/
Game::Game(GLFWwindow* aWindow)
  : mWindow(aWindow)
  , mLastFrameTime(0)
  , mParticleEmitterSystem(nullptr)
  , mShipControllerSystem(nullptr)
  , mPhysicsSystem(nullptr)
  , mShipRenderSystem(nullptr)
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

  // Register components.
  mScene.RegisterComponentType<Laser>(500);
  mScene.RegisterComponentType<ParticleEmitter>(1);
  mScene.RegisterComponentType<Physics>(1);
  mScene.RegisterComponentType<ShipController>(1);
  mScene.RegisterComponentType<Transform>(501);

  // Register systems.
  Signature sig;
  mScene.AddComponentToSignature<Transform>(sig);
  mScene.AddComponentToSignature<ShipController>(sig);
  mShipControllerSystem = mScene.RegisterSystemType<ShipControllerSystem>(sig);
  mShipRenderSystem = mScene.RegisterSystemType<ShipRenderSystem>(sig);

  mScene.RemoveComponentFromSignature<ShipController>(sig);
  mScene.AddComponentToSignature<Physics>(sig);
  mPhysicsSystem = mScene.RegisterSystemType<PhysicsSystem>(sig);

  mScene.RemoveComponentFromSignature<Physics>(sig);
  mScene.RemoveComponentFromSignature<Transform>(sig);
  mScene.AddComponentToSignature<ParticleEmitter>(sig);
  mParticleEmitterSystem = mScene.RegisterSystemType<ParticleEmitterSystem>(sig);

  mScene.RemoveComponentFromSignature<ParticleEmitter>(sig);
  mScene.AddComponentToSignature<Transform>(sig);
  mScene.AddComponentToSignature<Laser>(sig);
  mLaserSystem = mScene.RegisterSystemType<LaserSystem>(sig);

  // Create entities.
  auto ship = mScene.CreateEntity();
  mScene.AddComponentToEntity<Transform>(ship);
  mScene.AddComponentToEntity<ShipController>(ship);
  mScene.AddComponentToEntity<Physics>(ship);

  auto emitter = mScene.CreateEntity();
  mScene.AddComponentToEntity<ParticleEmitter>(emitter);
}

/******************************************************************************/
void Game::Run()
{
  // Run until instructed to close.
  while(!glfwWindowShouldClose(mWindow))
  {
    glfwSwapBuffers(mWindow);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto dt = glfwGetTime() - mLastFrameTime;

    mLaserSystem->Update(mScene);
    mParticleEmitterSystem->Update(mScene, mRandomDevice);
    mShipControllerSystem->Update(mScene, mInput);
    mPhysicsSystem->Update(mScene, dt);

    mLaserSystem->Render(mScene);
    mShipRenderSystem->Render(mScene);
    mParticleEmitterSystem->Render(mScene);

    mLastFrameTime = glfwGetTime();

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
