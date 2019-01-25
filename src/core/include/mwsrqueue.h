#pragma once

#include <mutex>
#include "throwassert.h"
#include <utility>
#include <condition_variable>

namespace Core {

// Simple multiple writer siingle reader queue implementation with infinite growth
template <class Collection>
class MWSRQueue {
  public:
    using T = typename Collection::value_type;
    
    void push_back(T&& it) {
      {
        std::unique_lock lock(mx);
        coll.push_back(std::move(it));
      }
      
      waitrd.notify_one();
    }
    
    std::pair<bool, T> pop_front() {
      std::unique_lock lock(mx);
      while (!coll.size() && !killFlag)
        waitrd.wait(lock);
      if (killFlag)
        return {false, T()};
      throw_assert(coll.size() > 0, "There should be at least one element in the queue");
      T ret = std::move(coll.front());
      coll.pop_front();
      lock.unlock();
      return {true, std::move(ret)};
    }
    
    void kill() {
      {
        std::unique_lock lock(mx);
        killFlag = true;
      }
      waitrd.notify_all();
    }

    size_t size() {
        std::unique_lock lock(mx);
        size_t size = coll.size();
        lock.unlock();
        return size;
    }
    
  private:
    std::mutex mx;
    std::condition_variable waitrd;
    Collection coll;
    bool killFlag = false;
};

}
