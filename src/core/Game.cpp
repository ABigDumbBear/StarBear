#include "Game.hpp"

#include <iostream>
#include <random>

#include "EntityFactory.hpp"
#include "MathUtil.hpp"

namespace StarBear {

/******************************************************************************/
Game::Game(GLFWwindow* aWindow)
  : mWindow(aWindow)
  , mLastFrameTime(0)
  , mCollisionSystem(nullptr)
  , mEnemySystem(nullptr)
  , mLaserSystem(nullptr)
  , mParticleEmitterSystem(nullptr)
  , mShipControllerSystem(nullptr)
  , mPhysicsSystem(nullptr)
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
  mScene.RegisterComponentType<Hitbox>(1000);
  mScene.RegisterComponentType<Enemy>(1000);
  mScene.RegisterComponentType<Laser>(1000);
  mScene.RegisterComponentType<ParticleEmitter>(1000);
  mScene.RegisterComponentType<Physics>(1000);
  mScene.RegisterComponentType<ShipController>(1000);
  mScene.RegisterComponentType<Transform>(1000);

  // Register systems.
  Signature sig;
  mScene.AddComponentToSignature<Transform>(sig);
  mScene.AddComponentToSignature<ShipController>(sig);
  mShipControllerSystem = mScene.RegisterSystemType<ShipControllerSystem>(sig);

  sig.reset();
  mScene.AddComponentToSignature<Physics>(sig);
  mPhysicsSystem = mScene.RegisterSystemType<PhysicsSystem>(sig);

  sig.reset();
  mScene.AddComponentToSignature<ParticleEmitter>(sig);
  mParticleEmitterSystem = mScene.RegisterSystemType<ParticleEmitterSystem>(sig);

  sig.reset();
  mScene.AddComponentToSignature<Transform>(sig);
  mScene.AddComponentToSignature<Laser>(sig);
  mLaserSystem = mScene.RegisterSystemType<LaserSystem>(sig);

  sig.reset();
  mScene.AddComponentToSignature<Enemy>(sig);
  mScene.AddComponentToSignature<Transform>(sig);
  mEnemySystem = mScene.RegisterSystemType<EnemySystem>(sig);

  sig.reset();
  mScene.AddComponentToSignature<Transform>(sig);
  mScene.AddComponentToSignature<Hitbox>(sig);
  mCollisionSystem = mScene.RegisterSystemType<CollisionSystem>(sig);

  // Create entities.
  CreateShip(mScene);
  CreateEmitter(mScene);

  auto enemy = CreateEnemy(mScene);
  mScene.GetComponentForEntity<Transform>(enemy).SetPosition(Vec3(10, 10, -50));
  mScene.GetComponentForEntity<Transform>(enemy).Scale(2.5, 2.5, 2.5);
  enemy = CreateEnemy(mScene);
  mScene.GetComponentForEntity<Transform>(enemy).SetPosition(Vec3(-10, 10, -50));
  mScene.GetComponentForEntity<Transform>(enemy).Scale(2.5, 2.5, 2.5);
  enemy = CreateEnemy(mScene);
  mScene.GetComponentForEntity<Transform>(enemy).SetPosition(Vec3(-10, -10, -50));
  mScene.GetComponentForEntity<Transform>(enemy).Scale(2.5, 2.5, 2.5);
  enemy = CreateEnemy(mScene);
  mScene.GetComponentForEntity<Transform>(enemy).SetPosition(Vec3(10, -10, -50));
  mScene.GetComponentForEntity<Transform>(enemy).Scale(2.5, 2.5, 2.5);
}

/******************************************************************************/
void Game::Run()
{
  std::random_device rd;

  // Run until instructed to close.
  while(!glfwWindowShouldClose(mWindow))
  {
    glfwSwapBuffers(mWindow);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto dt = glfwGetTime() - mLastFrameTime;

    mCollisionSystem->Update(mScene);
    mEnemySystem->Update(mScene, dt);
    mLaserSystem->Update(mScene, dt);
    mParticleEmitterSystem->Update(mScene, rd, dt);
    mShipControllerSystem->Update(mScene, mInput, dt);
    mPhysicsSystem->Update(mScene, dt);

    mLaserSystem->Render(mScene, mResourceMap);
    mShipControllerSystem->Render(mScene, mResourceMap);
    mEnemySystem->Render(mScene, mResourceMap);
    mParticleEmitterSystem->Render(mScene, mResourceMap);
    mCollisionSystem->Render(mScene, mResourceMap);

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
