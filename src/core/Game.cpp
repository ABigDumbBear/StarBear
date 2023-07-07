#include "Game.hpp"

#include <iostream>
#include <random>

#include "EntityFactory.hpp"

#include "Camera.hpp"
#include "Parent.hpp"
#include "RailMover.hpp"

namespace StarBear {

/******************************************************************************/
Game::Game(GLFWwindow *aWindow)
    : mWindow(aWindow), mLastFrameTime(0), mCameraSystem(nullptr),
      mCollisionSystem(nullptr), mParticleEmitterSystem(nullptr),
      mRailMoverSystem(nullptr), mShipControllerSystem(nullptr),
      mPhysicsSystem(nullptr) {
  // Set GLFW callbacks.
  glfwSetWindowUserPointer(mWindow, this);
  glfwSetErrorCallback([](int aError, const char *aDescription) {
    std::cout << "GLFW encountered an error: " << aDescription << std::endl;
  });

  glfwSetFramebufferSizeCallback(
      mWindow, [](GLFWwindow *aWindow, int aWidth, int aHeight) {
        auto game = static_cast<Game *>(glfwGetWindowUserPointer(aWindow));
        game->FramebufferSizeCallback(aWidth, aHeight);
      });

  glfwSetKeyCallback(mWindow, [](GLFWwindow *aWindow, int aKey, int aScancode,
                                 int aAction, int aMods) {
    auto game = static_cast<Game *>(glfwGetWindowUserPointer(aWindow));
    game->KeyCallback(aKey, aScancode, aAction, aMods);
  });

  // Register components.
  mScene.RegisterComponentType<Camera>(1000);
  mScene.RegisterComponentType<Hitbox>(1000);
  mScene.RegisterComponentType<ParticleEmitter>(1000);
  mScene.RegisterComponentType<Physics>(1000);
  mScene.RegisterComponentType<ShipController>(1000);
  mScene.RegisterComponentType<RailMover>(1000);
  mScene.RegisterComponentType<Parent>(1000);

  mScene.RegisterComponentType<KumaGL::Transform>(1000);

  // Register systems.
  KumaECS::Signature sig;
  mScene.AddComponentToSignature<KumaGL::Transform>(sig);
  mScene.AddComponentToSignature<ShipController>(sig);
  mShipControllerSystem = mScene.RegisterSystemType<ShipControllerSystem>(sig);

  sig.reset();
  mScene.AddComponentToSignature<Physics>(sig);
  mPhysicsSystem = mScene.RegisterSystemType<PhysicsSystem>(sig);

  sig.reset();
  mScene.AddComponentToSignature<KumaGL::Transform>(sig);
  mScene.AddComponentToSignature<ParticleEmitter>(sig);
  mParticleEmitterSystem =
      mScene.RegisterSystemType<ParticleEmitterSystem>(sig);

  sig.reset();
  mScene.AddComponentToSignature<KumaGL::Transform>(sig);
  mScene.AddComponentToSignature<Hitbox>(sig);
  mCollisionSystem = mScene.RegisterSystemType<CollisionSystem>(sig);

  sig.reset();
  mScene.AddComponentToSignature<KumaGL::Transform>(sig);
  mScene.AddComponentToSignature<RailMover>(sig);
  mRailMoverSystem = mScene.RegisterSystemType<RailMoverSystem>(sig);

  sig.reset();
  mScene.AddComponentToSignature<KumaGL::Transform>(sig);
  mScene.AddComponentToSignature<Camera>(sig);
  mCameraSystem = mScene.RegisterSystemType<CameraSystem>(sig);

  sig.reset();
  mScene.AddComponentToSignature<KumaGL::Transform>(sig);
  mScene.AddComponentToSignature<Parent>(sig);
  mParentSystem = mScene.RegisterSystemType<ParentSystem>(sig);

  // Create entities.
  auto ship = CreateShip(mScene);
  mScene.AddComponentToEntity<Parent>(ship);

  auto emitter = CreateEmitter(mScene);

  mScene.GetComponentForEntity<Parent>(ship).mChildren.insert(emitter);
  mScene.GetComponentForEntity<ParticleEmitter>(emitter).mRadius = 0.5;
  mScene.GetComponentForEntity<KumaGL::Transform>(emitter).Translate(
      KumaGL::Vec3(4.5, 0, 2));

  auto emitter2 = CreateEmitter(mScene);

  mScene.GetComponentForEntity<Parent>(ship).mChildren.insert(emitter2);
  mScene.GetComponentForEntity<ParticleEmitter>(emitter2).mRadius = 0.5;
  mScene.GetComponentForEntity<KumaGL::Transform>(emitter2).Translate(
      KumaGL::Vec3(-4.5, 0, 2));

  auto emitter3 = CreateEmitter(mScene);

  mScene.GetComponentForEntity<Parent>(ship).mChildren.insert(emitter3);
  mScene.GetComponentForEntity<ParticleEmitter>(emitter3).mRadius = 0.5;
  mScene.GetComponentForEntity<KumaGL::Transform>(emitter3).Translate(
      KumaGL::Vec3(4.5, -1.2, 2));

  auto emitter4 = CreateEmitter(mScene);

  mScene.GetComponentForEntity<Parent>(ship).mChildren.insert(emitter4);
  mScene.GetComponentForEntity<ParticleEmitter>(emitter4).mRadius = 0.5;
  mScene.GetComponentForEntity<KumaGL::Transform>(emitter4).Translate(
      KumaGL::Vec3(-4.5, -1.2, 2));

  auto camera = mScene.CreateEntity();
  mScene.AddComponentToEntity<KumaGL::Transform>(camera).SetPosition(
      KumaGL::Vec3(0, 0, 50));
  mScene.AddComponentToEntity<Camera>(camera);

  auto mover = mScene.CreateEntity();
  mScene.AddComponentToEntity<KumaGL::Transform>(mover);
  mScene.AddComponentToEntity<RailMover>(mover);
  mScene.AddComponentToEntity<Parent>(mover);

  mScene.GetComponentForEntity<Parent>(mover).mChildren.insert(ship);
  mScene.GetComponentForEntity<Parent>(mover).mChildren.insert(camera);
}

/******************************************************************************/
void Game::Run() {
  std::random_device rd;

  auto proj = KumaGL::Perspective(45, 1280, 720, 0.1, 1000);

  int frameCounter = 0;
  double frameTimer = 0;

  // Run until instructed to close.
  while (!glfwWindowShouldClose(mWindow)) {
    glfwPollEvents();
    glfwSwapBuffers(mWindow);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto dt = glfwGetTime() - mLastFrameTime;
    mLastFrameTime = glfwGetTime();

    mCollisionSystem->Update(mScene);
    mShipControllerSystem->Update(mScene, mInput, dt);
    mParticleEmitterSystem->Update(mScene, rd, dt);
    mPhysicsSystem->Update(mScene, dt);
    mRailMoverSystem->Update(mScene);
    mParentSystem->Update(mScene);

    for (const auto &entity : mCameraSystem->mEntities) {
      auto &transform = mScene.GetComponentForEntity<KumaGL::Transform>(entity);
      auto view = View(KumaGL::Vec3(0, 0, 1), KumaGL::Vec3(1, 0, 0),
                       transform.GetWorldPosition());

      mShipControllerSystem->Render(mScene, mResourceMap, view, proj);
      mCollisionSystem->Render(mScene, mResourceMap, view, proj);
      mParticleEmitterSystem->Render(mScene, mResourceMap, view, proj);
    }
  }
}

/******************************************************************************/
void Game::FramebufferSizeCallback(int aWidth, int aHeight) {
  glViewport(0, 0, aWidth, aHeight);
}

/******************************************************************************/
void Game::KeyCallback(int aKey, int aScancode, int aAction, int aMods) {
  if (aAction == GLFW_PRESS) {
    mInput.mPressedKeys.insert(aKey);
  } else if (aAction == GLFW_RELEASE) {
    mInput.mPressedKeys.erase(aKey);
  }
}

} // namespace StarBear
