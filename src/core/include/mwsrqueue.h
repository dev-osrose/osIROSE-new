#pragma once

#include <mutex>
#include "throw_assert.h"
#include <pair>
#include <utility>

namespace Core {

// Simple multiple writer siingle reader queue implementation with infinite growth
template <class Collection>
class MWSRQueue {
  public:
    using T = Collection::value_type;
    
    void push_back(T&& it) {
      {
        std::unique_lock lock(_mx);
        coll.push_back(std::move(it));
      }
      
      waitrd.notify_one();
    }
    
    std::pair<bool, T> pop_front() {
      std::unique_lock lock(_mx);
      while (!coll.size() && !_killFlag)
        waitrd.wait(lock);
      if (_killFlag)
        return {false, T()};
      throw_assert(coll.size() > 0, "There should be at least one element in the queue");
      T ret = std::move(coll.front());
      coll.pop_front();
      lock.unlock();
      return {true, std::move(ret)};
    }
    
    void kill() {
      {
        std::unique_lock lock(_mx);
        _killFlag = true;
      }
      waitrd.notify_all();
    }
    
  private:
    std::mutex _mx;
    std::condition_variable _waitrd;
    Collection _coll;
    bool _killFlag = false;
};

}
