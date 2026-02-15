#ifndef _HEALTH_SERVER_H_
#define _HEALTH_SERVER_H_

#include <asio.hpp>
#include <atomic>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace Core {

class HealthServer {
 public:
  using HealthCheck = std::function<std::pair<bool, std::string>()>;

  HealthServer();
  ~HealthServer();

  void start(uint16_t port);
  void stop();

  void addCheck(HealthCheck check);
  void setUnhealthy(const std::string& reason);

 private:
  void run();
  void handleAccept();
  void handleRequest(std::shared_ptr<asio::ip::tcp::socket> socket);

  uint16_t port_;
  std::atomic<bool> running_;
  std::atomic<bool> forcedUnhealthy_;
  std::string unhealthyReason_;
  std::mutex reasonMutex_;

  std::vector<HealthCheck> checks_;
  std::thread serverThread_;

  asio::io_context io_context_;
  std::unique_ptr<asio::ip::tcp::acceptor> acceptor_;
};

}  // namespace Core

#endif // _HEALTH_SERVER_H_
