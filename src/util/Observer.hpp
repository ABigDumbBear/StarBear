#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <functional>
#include <map>
#include <set>

namespace StarBear {

/*******************************************************************************
 * Interface classes
 ******************************************************************************/
class IObserver
{
  public:
    virtual ~IObserver() = default;
};

class ISignal
{
  public:
    virtual ~ISignal() = default;

    virtual void Disconnect(IObserver& aObserver) = 0;
};

/*******************************************************************************
 * Implementation classes
 ******************************************************************************/
class Observer : public IObserver
{
  template<class ...Args> friend class Signal;

  public:
    virtual ~Observer() override
    {
      for(auto& signal : mConnectedSignals)
      {
        signal->Disconnect(*this);
      }
    }

  private:
    void Add(ISignal& aSignal)
    {
      mConnectedSignals.insert(&aSignal);
    }

    std::set<ISignal*> mConnectedSignals;
};

template <typename ...Args>
class Signal : public ISignal
{
  public:
    void Notify(Args... args)
    {
      for(auto& funcData : mFunctionMap)
      {
        auto size = funcData.second.size();
        for(size_t i = 0; i < size; ++i)
        {
          funcData.second[i](args...);
        }
      }
    }

    void Connect(Observer& aObserver,
                 const std::function<void(Args...)>& aFunction)
    {
      auto funcList = mFunctionMap.find(&aObserver);
      if(funcList != mFunctionMap.end())
      {
        funcList->second.emplace_back(aFunction);
      }
      else
      {
        std::vector<std::function<void(Args...)>> newList;
        newList.emplace_back(aFunction);
        mFunctionMap.emplace(&aObserver, newList);
      }

      aObserver.Add(*this);
    }

    void Disconnect(IObserver& aObserver) override
    {
      auto funcList = mFunctionMap.find(&aObserver);
      if(funcList != mFunctionMap.end())
      {
        mFunctionMap.erase(funcList);
      }
    }

  private:
    std::map<IObserver*, std::vector<std::function<void (Args...)>>> mFunctionMap;
};

} // namespace StarBear

#endif
