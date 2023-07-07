#ifndef ENTITYFACTORY_HPP
#define ENTITYFACTORY_HPP

#include <KumaECS/ECS.hpp>

#include <KumaGL/Transform.hpp>

#include "Hitbox.hpp"
#include "ParticleEmitter.hpp"
#include "Physics.hpp"
#include "ShipController.hpp"

namespace StarBear {

/******************************************************************************/
inline KumaECS::Entity CreateShip(KumaECS::Scene &aScene) {
  auto entity = aScene.CreateEntity();
  aScene.AddComponentToEntity<KumaGL::Transform>(entity);
  aScene.AddComponentToEntity<ShipController>(entity);

  auto &hitbox = aScene.AddComponentToEntity<Hitbox>(entity);
  hitbox.mWidth = 10;
  hitbox.mHeight = 2;
  hitbox.x = -5;
  hitbox.y = -1;

  return entity;
}

/******************************************************************************/
inline KumaECS::Entity CreateEmitter(KumaECS::Scene &aScene) {
  auto entity = aScene.CreateEntity();
  aScene.AddComponentToEntity<KumaGL::Transform>(entity);
  aScene.AddComponentToEntity<ParticleEmitter>(entity).mEmissionRate = 500;

  return entity;
}

} // namespace StarBear

#endif
