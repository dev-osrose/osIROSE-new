#include "health_server.h"
#include <sstream>
#include "logconsole.h"

namespace Core {

HealthServer::HealthServer() : port_(0), running_(false), forcedUnhealthy_(false) {}

HealthServer::~HealthServer() { stop(); }

void HealthServer::start(uint16_t port) {
  port_ = port;
  running_ = true;
  serverThread_ = std::thread(&HealthServer::run, this);
}

void HealthServer::stop() {
  running_ = false;
  io_context_.stop();
  if (serverThread_.joinable()) {
    serverThread_.join();
  }
}

void HealthServer::addCheck(HealthCheck check) { checks_.push_back(check); }

void HealthServer::setUnhealthy(const std::string& reason) {
  std::lock_guard<std::mutex> lock(reasonMutex_);
  forcedUnhealthy_ = true;
  unhealthyReason_ = reason;
}

void HealthServer::run() {
  try {
    asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), port_);
    acceptor_ = std::make_unique<asio::ip::tcp::acceptor>(io_context_, endpoint);

    if (auto log = Core::CLog::GetLogger(Core::log_type::GENERAL).lock()) {
      log->info("Health check listening on port {}", port_);
    }

    handleAccept();
    io_context_.run();
  } catch (std::exception& e) {
    if (auto log = Core::CLog::GetLogger(Core::log_type::GENERAL).lock()) {
      log->error("Health server error: {}", e.what());
    }
  }
}

void HealthServer::handleAccept() {
  auto socket = std::make_shared<asio::ip::tcp::socket>(io_context_);
  acceptor_->async_accept(*socket, [this, socket](const asio::error_code& error) {
    if (!error) {
      handleRequest(socket);
    }
    if (running_) {
      handleAccept();
    }
  });
}

void HealthServer::handleRequest(std::shared_ptr<asio::ip::tcp::socket> socket) {
  auto buffer = std::make_shared<std::array<char, 1024>>();
  socket->async_read_some(asio::buffer(*buffer), [this, socket, buffer](const asio::error_code& error, std::size_t bytes_transferred) {
    if (error) return;

    std::string request(buffer->data(), bytes_transferred);
    if (request.find("GET /health") == std::string::npos) {
      std::string response404 = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nConnection: close\r\n\r\n";
      asio::write(*socket, asio::buffer(response404));
      return;
    }

    bool healthy = !forcedUnhealthy_;
    std::string reason;

    if (forcedUnhealthy_) {
      std::lock_guard<std::mutex> lock(reasonMutex_);
      reason = unhealthyReason_;
    } else {
      for (auto& check : checks_) {
        auto result = check();
        if (!result.first) {
          healthy = false;
          reason = result.second;
          break;
        }
      }
    }

    std::stringstream response;
    if (healthy) {
      std::string body = "{ \"status\": \"ok\" }";
      response << "HTTP/1.1 200 OK\r\n";
      response << "Content-Type: application/json\r\n";
      response << "Content-Length: " << body.length() << "\r\n";
      response << "Connection: close\r\n\r\n";
      response << body;
    } else {
      std::string body = "{ \"status\": \"unhealthy\", \"reason\": \"" + reason + "\" }";
      response << "HTTP/1.1 503 Service Unavailable\r\n";
      response << "Content-Type: application/json\r\n";
      response << "Content-Length: " << body.length() << "\r\n";
      response << "Connection: close\r\n\r\n";
      response << body;
    }

    std::string responseStr = response.str();
    asio::write(*socket, asio::buffer(responseStr));
  });
}

}  // namespace Core
