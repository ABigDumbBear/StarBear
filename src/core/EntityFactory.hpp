#ifndef ENTITYFACTORY_HPP
#define ENTITYFACTORY_HPP

#include "ECS.hpp"

#include "Enemy.hpp"
#include "Hitbox.hpp"
#include "Laser.hpp"
#include "ParticleEmitter.hpp"
#include "Physics.hpp"
#include "ShipController.hpp"
#include "Transform.hpp"

namespace StarBear {

/******************************************************************************/
inline Entity CreateShip(Scene& aScene)
{
  auto entity = aScene.CreateEntity();
  aScene.AddComponentToEntity<Transform>(entity);
  aScene.AddComponentToEntity<ShipController>(entity);
  aScene.AddComponentToEntity<Physics>(entity);

  return entity;
}

/******************************************************************************/
inline Entity CreateLaser(Scene& aScene)
{
  auto entity = aScene.CreateEntity();
  aScene.AddComponentToEntity<Laser>(entity);
  aScene.AddComponentToEntity<Transform>(entity);
  aScene.AddComponentToEntity<Hitbox>(entity);

  return entity;
}

/******************************************************************************/
inline Entity CreateEnemy(Scene& aScene)
{
  auto entity = aScene.CreateEntity();
  aScene.AddComponentToEntity<Enemy>(entity).mHealth = 10;
  aScene.AddComponentToEntity<Transform>(entity);

  auto& hitbox = aScene.AddComponentToEntity<Hitbox>(entity);
  hitbox.mHeight = 2.5;
  hitbox.mWidth = 5;
  hitbox.x = -2.5;
  hitbox.y = -1;

  return entity;
}

/******************************************************************************/
inline Entity CreateEmitter(Scene& aScene)
{
  auto entity = aScene.CreateEntity();
  aScene.AddComponentToEntity<ParticleEmitter>(entity);

  return entity;
}

} // namespace StarBear

#endif
