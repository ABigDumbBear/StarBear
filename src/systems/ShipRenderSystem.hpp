#ifndef SHIPRENDERSYSTEM_HPP
#define SHIPRENDERSYSTEM_HPP

#include "ECS.hpp"

#include "Model.hpp"
#include "Shader.hpp"

namespace StarBear {

class ShipRenderSystem : public System
{
  public:
    ShipRenderSystem();

    void Render(Scene& aScene);

  private:
    Model mModel;
    Shader mShader;
};

} // namespace StarBear

#endif
