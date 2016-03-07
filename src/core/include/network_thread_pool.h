#ifndef thread_pool_h__
#define thread_pool_h__
#include <thread>
#include <asio.hpp>
#include <queue>
#include <mutex>

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
  uint32_t Get_CPU_Count() {
    return std::thread::hardware_concurrency()
               ? std::thread::hardware_concurrency()
               : 1;
  }
  void Shutdown() {
    io_work_.reset();

    for (uint32_t idx = 0; idx < threads_running_; ++idx) {
      io_thread_[idx].join();
    }
    threads_running_ = 0;
    io_service_.stop();
  }

 protected:
 private:
  NetworkThreadPool(uint16_t maxthreads) : io_work_(new asio_worker::element_type(io_service_)) {
    uint16_t core_count = Get_CPU_Count();

    core_count *= 2;

    if(maxthreads != 0 && core_count > maxthreads)
      core_count = maxthreads;

    if (core_count > 512)
      core_count = 512;
    else if (core_count == 0)
      core_count = 1;

    threads_running_ = core_count;
    printf("Using %i threads\n", threads_running_);
    for (uint32_t idx = 0; idx < threads_running_; ++idx) {
      io_thread_[idx] = std::thread([this]() { io_service_.run(); }); //todo(raven): change this to poll and loop while we are active.
    }
  }

  ~NetworkThreadPool() { Shutdown(); }

  uint16_t threads_running_;
  std::thread io_thread_[512];
  asio::io_service io_service_;
  asio_worker io_work_;
  static NetworkThreadPool* instance_;
};
}

#endif  // thread_pool_h__
