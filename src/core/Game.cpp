#include "Game.hpp"

#include <iostream>

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
  mScene.RegisterComponentType<Hitbox>(502);
  mScene.RegisterComponentType<Enemy>(1);
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
  mScene.GetComponentForEntity<Transform>(enemy).SetPosition(Vec3(0, 0, -50));
  mScene.GetComponentForEntity<Hitbox>(enemy).mHeight = 50;
  mScene.GetComponentForEntity<Hitbox>(enemy).mWidth = 50;
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

    mCollisionSystem->Update(mScene);
    mLaserSystem->Update(mScene);
    mParticleEmitterSystem->Update(mScene, mRandomDevice, dt);
    mShipControllerSystem->Update(mScene, mInput, dt);
    mEnemySystem->Update(mScene, dt);
    mPhysicsSystem->Update(mScene, dt);

    mLaserSystem->Render(mScene);
    mShipRenderSystem->Render(mScene);
    mEnemySystem->Render(mScene);
    //mParticleEmitterSystem->Render(mScene);
    mCollisionSystem->Render(mScene);

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
