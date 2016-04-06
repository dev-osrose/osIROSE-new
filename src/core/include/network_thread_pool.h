#ifndef _thread_pool_h_
#define _thread_pool_h_
#include <thread>
#include <asio.hpp>
#include <queue>
#include <mutex>
#include <atomic>

namespace Core {

class NetworkThreadPool {
  typedef std::unique_ptr<asio::io_service::work> asio_worker;

 public:
  static NetworkThreadPool& GetInstance(uint16_t maxthreads = 0) {
    if (instance_ == nullptr) instance_ = new NetworkThreadPool(maxthreads);
    return *instance_;
  }

  static void DeleteInstance() {
    if (instance_ != nullptr) delete instance_;
  }

  asio::io_service* Get_IO_Service() { return &io_service_; }
  uint16_t GetThreadCount() { return threads_running_; }

 private:
  NetworkThreadPool(uint16_t maxthreads) : io_work_(new asio_worker::element_type(io_service_)) {
    uint16_t core_count = std::thread::hardware_concurrency()
               ? std::thread::hardware_concurrency()
               : 1;;

    core_count *= 2;

    if(maxthreads != 0 && core_count > maxthreads)
      core_count = maxthreads;

    if (core_count > 512)
      core_count = 512;
    else if (core_count == 0)
      core_count = 1;

    threads_running_ = core_count;
    for (uint32_t idx = 0; idx < core_count; ++idx) {
      io_thread_[idx] = std::thread([this]() {
        ++threads_running_;
        io_service_.run();
        --threads_running_;
      }); //todo(raven): change this to poll and loop while we are active.
    }
  }

  ~NetworkThreadPool() { Shutdown(); }

  void Shutdown() {
    io_work_.reset();

    for (int idx = 0; idx < threads_running_; ++idx) {
      io_thread_[idx].join();
    }
//    threads_running_ = 0;
    io_service_.stop();
  }

  std::atomic<int> threads_running_;
  std::thread io_thread_[512];
  asio::io_service io_service_;
  asio_worker io_work_;
  static NetworkThreadPool* instance_;
};
}

#endif  // __thread_pool_h__
