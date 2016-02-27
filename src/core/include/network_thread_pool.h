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
    static NetworkThreadPool instance;
    return instance;
  }

  asio::io_service* Get_IO_Service() { return &io_service_; }
  uint32_t Get_CPU_Count() { return std::thread::hardware_concurrency(); }

 protected:
 private:
  NetworkThreadPool() : io_work_(new asio_worker::element_type(io_service_)) {
    uint32_t core_count = Get_CPU_Count();
    for (uint32_t idx = 0; idx < core_count; ++idx) {
      io_thread_[idx] = std::thread([this]() { io_service_.run(); });
    }
  }

  ~NetworkThreadPool() {
    io_work_.reset();

    uint32_t core_count = Get_CPU_Count();
    for (uint32_t idx = 0; idx < core_count; ++idx) {
      io_thread_[idx].join();
    }

    io_service_.stop();
  }

  std::thread io_thread_[256];
  asio::io_service io_service_;
  asio_worker io_work_;
};
}

#endif  // thread_pool_h__
