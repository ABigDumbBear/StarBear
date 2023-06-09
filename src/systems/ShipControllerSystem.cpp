#include "ShipControllerSystem.hpp"

#include <GLFW/glfw3.h>

#include <KumaGL/MathUtil.hpp>
#include <KumaGL/Transform.hpp>

#include "EntityFactory.hpp"

#include "ShipController.hpp"

namespace StarBear {

float easeInOutBack(double x) {
  float c1 = 1.70158;
  float c2 = c1 * 1.525;

  return x < 0.5 ? (std::pow(2.0 * x, 2.0) * ((c2 + 1.0) * 2.0 * x - c2)) / 2.0
                 : (std::pow(2.0 * x - 2.0, 2.0) *
                        ((c2 + 1.0) * (x * 2.0 - 2.0) + c2) +
                    2.0) /
                       2.0;
}

/******************************************************************************/
void ShipControllerSystem::Update(KumaECS::Scene &aScene, const Input &aInput,
                                  double dt) {
  for (const auto &entity : mEntities) {
    auto &controller = aScene.GetComponentForEntity<ShipController>(entity);
    auto &transform = aScene.GetComponentForEntity<KumaGL::Transform>(entity);

    // Handle directional input
    if (aInput.mPressedKeys.count(GLFW_KEY_W)) {
      controller.mTargetPos.y += 1;
    }
    if (aInput.mPressedKeys.count(GLFW_KEY_A)) {
      controller.mTargetPos.x -= 1;
    }
    if (aInput.mPressedKeys.count(GLFW_KEY_S)) {
      controller.mTargetPos.y -= 1;
    }
    if (aInput.mPressedKeys.count(GLFW_KEY_D)) {
      controller.mTargetPos.x += 1;
    }

    // Handle rolling input
    if (aInput.mPressedKeys.count(GLFW_KEY_J)) {
      controller.mState = ShipState::eROLLING;
    }

    auto newPos = transform.GetLocalPosition();
    newPos = Lerp(newPos, controller.mTargetPos, 0.3);
    newPos.z = 0;
    transform.SetPosition(newPos);

    switch (controller.mState) {
    case ShipState::eROLLING: {
      controller.mRollRotation += dt;
      transform.SetRotation(0, 0,
                            easeInOutBack(controller.mRollRotation) * 360);

      if (controller.mRollRotation >= 1) {
        controller.mRollRotation = 0;
        controller.mState = ShipState::eDEFAULT;
        transform.SetRotation(0, 0, 0);
      }
      break;
    }
    case ShipState::eDEFAULT:
    default: {
      break;
    }
    }
  }
}

/******************************************************************************/
void ShipControllerSystem::Render(KumaECS::Scene &aScene, ResourceMap &aMap,
                                  const KumaGL::Mat4 &aView,
                                  const KumaGL::Mat4 &aProj) {
  // Store the model matrix for each ship.
  std::vector<KumaGL::Mat4> modelMatrices;
  for (const auto &entity : mEntities) {
    auto &transform = aScene.GetComponentForEntity<KumaGL::Transform>(entity);
    modelMatrices.emplace_back(transform.GetMatrix());
  }

  auto &model = aMap.GetModel(ModelType::eSPITFIRE);
  for (const auto &mesh : model.GetMeshes()) {
    glBindBuffer(GL_ARRAY_BUFFER, mesh.GetInstanceBufferID());
    glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(KumaGL::Mat4),
                 modelMatrices.data(), GL_DYNAMIC_DRAW);
  }

  // Set shader uniforms and draw the model.
  auto &shader = aMap.GetShader(ShaderType::eTEXTURED_MESH);
  shader.Use();
  shader.SetMat4("viewMatrix", aView);
  shader.SetMat4("projectionMatrix", aProj);

  model.DrawInstanced(mEntities.size());
}

} // namespace StarBear
