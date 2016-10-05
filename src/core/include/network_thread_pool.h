// Copyright 2016 Chirstopher Torres (Raven), L3nn0x
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
// http ://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _thread_pool_h_
#define _thread_pool_h_
#include <thread>
#include <asio.hpp>
#include <queue>
#include <mutex>
#include <atomic>
#include <bitset>

namespace Core {
#define MAX_NETWORK_THREADS 512

class NetworkThreadPool {
  typedef std::unique_ptr<asio::io_context::work> asio_worker;

 public:
  static NetworkThreadPool& GetInstance(uint16_t maxthreads = 0) {
    if (instance_ == nullptr) instance_ = new NetworkThreadPool(maxthreads);
    return *instance_;
  }

  static void DeleteInstance() {
    if (instance_ != nullptr) {
      delete instance_;
      instance_ = nullptr;
    }
  }

  asio::io_context* Get_IO_Service() { return &io_service_; }
  uint16_t GetThreadCount() { return threads_active_.count(); }

 private:
  NetworkThreadPool(uint16_t maxthreads) : io_work_(new asio_worker::element_type(io_service_)) {
    uint16_t core_count = std::thread::hardware_concurrency()
               ? std::thread::hardware_concurrency()
               : 1;;

    core_count *= 2;

    if(maxthreads != 0 && core_count > maxthreads)
      core_count = maxthreads;

    if (core_count > MAX_NETWORK_THREADS)
      core_count = MAX_NETWORK_THREADS;
    else if (core_count <= 0)
      core_count = 1;

    for (uint32_t idx = 0; idx < core_count; ++idx) {
      threads_active_.set(idx);
      
      io_thread_[idx] = std::thread([this, idx]() {
        int index = idx;
        while(threads_active_.test(index)) io_service_.poll();
      });
    }
  }

  ~NetworkThreadPool() { Shutdown(); }

  void Shutdown() {
    threads_active_.reset();
    io_work_.reset();
    for (int idx = 0; idx < MAX_NETWORK_THREADS; ++idx) {
      if( true == io_thread_[idx].joinable() )
        io_thread_[idx].join();
    }
    
    io_service_.stop();
  }

  std::bitset<MAX_NETWORK_THREADS> threads_active_;
  std::thread io_thread_[MAX_NETWORK_THREADS];
  asio::io_context io_service_;
  asio_worker io_work_;
  static NetworkThreadPool* instance_;
};
}

#endif  // __thread_pool_h__
