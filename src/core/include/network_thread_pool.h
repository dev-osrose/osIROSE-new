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
  static NetworkThreadPool& GetInstance() {
    if (instance_ == nullptr) instance_ = new NetworkThreadPool();
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

    uint32_t core_count = Get_CPU_Count();
    core_count *= 2;

    if (core_count > 512)
      core_count = 512;
    else if (core_count == 0)
      core_count = 1;

    for (uint32_t idx = 0; idx < core_count; ++idx) {
      io_thread_[idx].join();
    }

    io_service_.stop();
  }

 protected:
 private:
  NetworkThreadPool() : io_work_(new asio_worker::element_type(io_service_)) {
    uint32_t core_count = Get_CPU_Count();

    core_count *= 2;

    if (core_count > 512)
      core_count = 512;
    else if (core_count == 0)
      core_count = 1;

    printf("Using %i threads\n", core_count);
    for (uint32_t idx = 0; idx < core_count; ++idx) {
      io_thread_[idx] = std::thread([this]() { io_service_.run(); });
    }
  }

  ~NetworkThreadPool() { Shutdown(); }

  std::thread io_thread_[512];
  asio::io_service io_service_;
  asio_worker io_work_;
  static NetworkThreadPool* instance_;
};
}

#endif  // thread_pool_h__
