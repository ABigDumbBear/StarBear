#include "Game.hpp"

#include "MathUtil.hpp"
#include "Transform.hpp"

namespace StarBear {

/******************************************************************************/
Game::Game(GLFWwindow* aWindow)
  : mWindow(aWindow)
{
  mShader.LoadFromFiles("resources/shaders/Ship.vert",
                        "resources/shaders/Ship.frag");
  mModel.LoadFromFile("resources/models/Spitfire/OBJ/Spitfire.obj");
}

/******************************************************************************/
void Game::Run()
{
  KumaGL::Transform modelTransform;
  modelTransform.SetPosition(KumaGL::Vec3(0, 0, -10));
  mShader.Use();
  mShader.SetMat4("modelMatrix", modelTransform.GetMatrix());
  mShader.SetMat4("viewMatrix", KumaGL::View(KumaGL::Vec3(0, 0, 1),
                                             KumaGL::Vec3(1, 0, 0),
                                             KumaGL::Vec3(0, 0, 0)));
  mShader.SetMat4("projectionMatrix", KumaGL::Perspective(45, 1280, 720, 0.1, 100));

  // Run until instructed to close.
  while(!glfwWindowShouldClose(mWindow))
  {
    glfwSwapBuffers(mWindow);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    modelTransform.SetRotation(0, sin(glfwGetTime()) * 100, 0);
    mShader.SetMat4("modelMatrix", modelTransform.GetMatrix());

    mModel.Draw(mShader);

    glfwPollEvents();
  }
}

} // namespace StarBear
