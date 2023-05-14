#ifndef GAME_HPP
#define GAME_HPP

#include <random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ECS.hpp"
#include "Input.hpp"

#include "CollisionSystem.hpp"
#include "EnemySystem.hpp"
#include "ParticleEmitterSystem.hpp"
#include "PhysicsSystem.hpp"
#include "ShipControllerSystem.hpp"
#include "ShipRenderSystem.hpp"
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

    Scene mScene;

    std::random_device mRandomDevice;
    double mLastFrameTime;

    CollisionSystem* mCollisionSystem;
    EnemySystem* mEnemySystem;
    LaserSystem* mLaserSystem;
    ParticleEmitterSystem* mParticleEmitterSystem;
    ShipControllerSystem* mShipControllerSystem;
    PhysicsSystem* mPhysicsSystem;
    ShipRenderSystem* mShipRenderSystem;
};

} // namespace StarBear

#endif
