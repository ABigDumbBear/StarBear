#ifndef GAME_HPP
#define GAME_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ECS.hpp"
#include "Input.hpp"

#include "ResourceMap.hpp"

#include "CameraSystem.hpp"
#include "CollisionSystem.hpp"
#include "EnemySystem.hpp"
#include "ParentSystem.hpp"
#include "ParticleEmitterSystem.hpp"
#include "PhysicsSystem.hpp"
#include "RailMoverSystem.hpp"
#include "ShipControllerSystem.hpp"
#include "LaserSystem.hpp"

namespace StarBear {

class Game
{
  public:
    Game(GLFWwindow* aWindow);

    void Run();

    void FramebufferSizeCallback(int aWidth, int aHeight);
    void KeyCallback(int aKey, int aScancode, int aAction, int aMods);

  private:
    GLFWwindow* mWindow;
    Input mInput;

    ResourceMap mResourceMap;

    Scene mScene;
    double mLastFrameTime;

    CameraSystem* mCameraSystem;
    CollisionSystem* mCollisionSystem;
    EnemySystem* mEnemySystem;
    LaserSystem* mLaserSystem;
    ParentSystem* mParentSystem;
    ParticleEmitterSystem* mParticleEmitterSystem;
    RailMoverSystem* mRailMoverSystem;
    ShipControllerSystem* mShipControllerSystem;
    PhysicsSystem* mPhysicsSystem;
};

} // namespace StarBear

#endif
