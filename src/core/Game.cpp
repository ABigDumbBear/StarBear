#include "Game.hpp"

#include <iostream>
#include <random>

#include "EntityFactory.hpp"
#include "MathUtil.hpp"

#include "Camera.hpp"
#include "RailMover.hpp"

namespace StarBear {

/******************************************************************************/
Game::Game(GLFWwindow* aWindow)
  : mWindow(aWindow)
  , mLastFrameTime(0)
  , mCameraSystem(nullptr)
  , mCollisionSystem(nullptr)
  , mEnemySystem(nullptr)
  , mLaserSystem(nullptr)
  , mParticleEmitterSystem(nullptr)
  , mRailMoverSystem(nullptr)
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
  mScene.RegisterComponentType<Camera>(1000);
  mScene.RegisterComponentType<Hitbox>(1000);
  mScene.RegisterComponentType<Enemy>(1000);
  mScene.RegisterComponentType<Laser>(1000);
  mScene.RegisterComponentType<ParticleEmitter>(1000);
  mScene.RegisterComponentType<Physics>(1000);
  mScene.RegisterComponentType<ShipController>(1000);
  mScene.RegisterComponentType<Transform>(1000);
  mScene.RegisterComponentType<RailMover>(1000);

  // Register systems.
  Signature sig;
  mScene.AddComponentToSignature<Transform>(sig);
  mScene.AddComponentToSignature<ShipController>(sig);
  mShipControllerSystem = mScene.RegisterSystemType<ShipControllerSystem>(sig);

  sig.reset();
  mScene.AddComponentToSignature<Physics>(sig);
  mPhysicsSystem = mScene.RegisterSystemType<PhysicsSystem>(sig);

  sig.reset();
  mScene.AddComponentToSignature<Transform>(sig);
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

  sig.reset();
  mScene.AddComponentToSignature<Transform>(sig);
  mScene.AddComponentToSignature<RailMover>(sig);
  mRailMoverSystem = mScene.RegisterSystemType<RailMoverSystem>(sig);

  sig.reset();
  mScene.AddComponentToSignature<Transform>(sig);
  mScene.AddComponentToSignature<Camera>(sig);
  mCameraSystem = mScene.RegisterSystemType<CameraSystem>(sig);

  // Create entities.
  auto ship = CreateShip(mScene);
  auto emitter = CreateEmitter(mScene);

  auto enemy = CreateEnemy(mScene);
  mScene.GetComponentForEntity<Transform>(enemy).SetPosition(Vec3(10, 10, -50));
  //mScene.GetComponentForEntity<Transform>(enemy).Scale(2.5, 2.5, 2.5);
  enemy = CreateEnemy(mScene);
  mScene.GetComponentForEntity<Transform>(enemy).SetPosition(Vec3(-10, 10, -50));
  //mScene.GetComponentForEntity<Transform>(enemy).Scale(2.5, 2.5, 2.5);
  enemy = CreateEnemy(mScene);
  mScene.GetComponentForEntity<Transform>(enemy).SetPosition(Vec3(-10, -10, -50));
  //mScene.GetComponentForEntity<Transform>(enemy).Scale(2.5, 2.5, 2.5);
  enemy = CreateEnemy(mScene);
  mScene.GetComponentForEntity<Transform>(enemy).SetPosition(Vec3(10, -10, -50));
  //mScene.GetComponentForEntity<Transform>(enemy).Scale(2.5, 2.5, 2.5);

  auto camera = mScene.CreateEntity();
  mScene.AddComponentToEntity<Transform>(camera).SetPosition(Vec3(0, 0, 50));
  mScene.AddComponentToEntity<Camera>(camera);

  auto mover = mScene.CreateEntity();
  mScene.AddComponentToEntity<Transform>(mover);
  auto& railMover = mScene.AddComponentToEntity<RailMover>(mover);
  railMover.mChildren.insert(ship);
  railMover.mChildren.insert(camera);
  railMover.mChildren.insert(emitter);
}

/******************************************************************************/
void Game::Run()
{
  std::random_device rd;

  auto proj = Perspective(45, 1280, 720, 0.1, 1000);

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
    mRailMoverSystem->Update(mScene);

    for(const auto& entity : mCameraSystem->mEntities)
    {
      auto& transform = mScene.GetComponentForEntity<Transform>(entity);
      auto view = View(Vec3(0, 0, 1), Vec3(1, 0, 0), transform.GetMatrix() * Vec3(0, 0, 0));

      mLaserSystem->Render(mScene, mResourceMap, view, proj);
      mShipControllerSystem->Render(mScene, mResourceMap, view, proj);
      mEnemySystem->Render(mScene, mResourceMap, view, proj);
      mParticleEmitterSystem->Render(mScene, mResourceMap, view, proj);
      mCollisionSystem->Render(mScene, mResourceMap, view, proj);
    }

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
