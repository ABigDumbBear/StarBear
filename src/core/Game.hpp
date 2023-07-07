#ifndef GAME_HPP
#define GAME_HPP

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <KumaECS/ECS.hpp>

#include "Input.hpp"

#include "ResourceMap.hpp"

#include "CameraSystem.hpp"
#include "CollisionSystem.hpp"
#include "ParentSystem.hpp"
#include "ParticleEmitterSystem.hpp"
#include "PhysicsSystem.hpp"
#include "RailMoverSystem.hpp"
#include "ShipControllerSystem.hpp"

namespace StarBear {

class Game {
public:
  Game(GLFWwindow *aWindow);

  void Run();

  void FramebufferSizeCallback(int aWidth, int aHeight);
  void KeyCallback(int aKey, int aScancode, int aAction, int aMods);

private:
  GLFWwindow *mWindow;
  Input mInput;

  ResourceMap mResourceMap;

  KumaECS::Scene mScene;
  double mLastFrameTime;

  CameraSystem *mCameraSystem;
  CollisionSystem *mCollisionSystem;
  ParentSystem *mParentSystem;
  ParticleEmitterSystem *mParticleEmitterSystem;
  RailMoverSystem *mRailMoverSystem;
  ShipControllerSystem *mShipControllerSystem;
  PhysicsSystem *mPhysicsSystem;
};

} // namespace StarBear

#endif
