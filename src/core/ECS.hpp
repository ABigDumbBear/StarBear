#ifndef ECS_HPP
#define ECS_HPP

#include <bitset>
#include <cassert>
#include <cstddef>
#include <memory>
#include <queue>
#include <set>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace StarBear {

/*******************************************************************************
 * Definitions
 ******************************************************************************/
const size_t MAX_COMPONENT_TYPES = 64;

using Entity = size_t;
using EntitySet = std::set<Entity>;
using Signature = std::bitset<MAX_COMPONENT_TYPES>;

/*******************************************************************************
 * System
 ******************************************************************************/
class System
{
  protected:
    EntitySet mEntities;
};

/*******************************************************************************
 * ComponentMap
 ******************************************************************************/
class IComponentMap { };

template <typename T>
class ComponentMap : public IComponentMap
{
  public:
    ComponentMap<T>(size_t aMax)
      : mSize(0)
    {
      mComponents.reserve(aMax);
      for(size_t i = 0; i < aMax; ++i)
      {
        mComponents.emplace_back();
      }
    }

    void AddComponent(Entity aEntity)
    {
      assert(mSize < mComponents.size());
      assert(mEntityToIndexMap.find(aEntity) == mEntityToIndexMap.end());

      mEntityToIndexMap[aEntity] = mSize;
      mIndexToEntityMap[mSize] = aEntity;
      ++mSize;
    }

    void RemoveComponent(Entity aEntity)
    {
      assert(mEntityToIndexMap.find(aEntity) != mEntityToIndexMap.end());

      mComponents[mEntityToIndexMap[aEntity]] = mComponents.back();
      mEntityToIndexMap[mIndexToEntityMap[mSize]] = mEntityToIndexMap[aEntity];
      --mSize;
    }

    T& GetComponent(Entity aEntity)
    {
      assert(mEntityToIndexMap.find(aEntity) != mEntityToIndexMap.end());
      return mComponents[mEntityToIndexMap[aEntity]];
    }

  private:
    std::unordered_map<Entity, size_t> mEntityToIndexMap;
    std::unordered_map<size_t, Entity> mIndexToEntityMap;
    std::vector<T> mComponents;

    size_t mSize;
};

/*******************************************************************************
 * Scene
 ******************************************************************************/
class Scene
{
  public:
    Scene(size_t aMaxEntities)
    {
      for(size_t i = 0; i < aMaxEntities; ++i)
      {
        mAvailableEntities.push(i);
        mEntitySignatures.emplace_back();
      }
    }

    Entity CreateEntity()
    {
      assert(!mAvailableEntities.empty());

      Entity entity = mAvailableEntities.front();
      mAvailableEntities.pop();
      return entity;
    }

    void DestroyEntity(Entity aEntity)
    {
      mAvailableEntities.push(aEntity);
      mEntitySignatures[aEntity].reset();
    }

    template<typename T>
    void RegisterComponentType(size_t aMax)
    {
      auto name = typeid(T).name();
      assert(mComponentToIndexMap.find(name) == mComponentToIndexMap.end());

      mComponentToIndexMap[name] = mComponentMaps.size();
      mComponentMaps.emplace_back(std::make_unique<ComponentMap<T>>(aMax));
    }

    template<typename T>
    void AddComponentToEntity(Entity aEntity)
    {
      auto name = typeid(T).name();
      assert(mComponentToIndexMap.find(name) != mComponentToIndexMap.end());

      auto componentMap = mComponentMaps[mComponentToIndexMap[name]].get();
      static_cast<ComponentMap<T>*>(componentMap)->AddComponent(aEntity);
      mEntitySignatures[aEntity].set(mComponentToIndexMap[name]);
    }

    template<typename T>
    void RemoveComponentFromEntity(Entity aEntity)
    {
      auto name = typeid(T).name();
      assert(mComponentToIndexMap.find(name) != mComponentToIndexMap.end());

      auto componentMap = mComponentMaps[mComponentToIndexMap[name]].get();
      static_cast<ComponentMap<T>*>(componentMap)->RemoveComponent(aEntity);
      mEntitySignatures[aEntity].reset(mComponentToIndexMap[name]);
    }

    template<typename T>
    T& GetComponentForEntity(Entity aEntity)
    {
      auto name = typeid(T).name();
      assert(mComponentToIndexMap.find(name) != mComponentToIndexMap.end());

      auto componentMap = mComponentMaps[mComponentToIndexMap[name]].get();
      return static_cast<ComponentMap<T>*>(componentMap)->GetComponent(aEntity);
    }

    template<typename T>
    void RegisterSystemType(const Signature& aSignature)
    {
      auto name = typeid(T).name();
      assert(mSystemToIndexMap.find(name) == mSystemToIndexMap.end());

      mSystemToIndexMap.emplace(name, mSystemSignatures.size());
      mSystemSignatures.emplace_back(aSignature);
    }

  private:
    std::queue<Entity> mAvailableEntities;
    std::vector<Signature> mEntitySignatures;

    std::unordered_map<const char*, size_t> mSystemToIndexMap;
    std::vector<Signature> mSystemSignatures;

    std::unordered_map<const char*, size_t> mComponentToIndexMap;
    std::vector<std::unique_ptr<IComponentMap>> mComponentMaps;
};

} // namespace StarBear

#endif
