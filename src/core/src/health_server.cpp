#include "health_server.h"
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include "logconsole.h"

namespace Core {

HealthServer::HealthServer() : port_(0), running_(false), forcedUnhealthy_(false), listenSocket_(-1) {}

HealthServer::~HealthServer() { stop(); }

void HealthServer::start(uint16_t port) {
  port_ = port;
  running_ = true;
  serverThread_ = std::thread(&HealthServer::run, this);
}

void HealthServer::stop() {
  running_ = false;
  if (listenSocket_ != -1) {
    // Trigger a wake up of the accept call by connecting to it or using shutdown
    shutdown(listenSocket_, SHUT_RDWR);
    close(listenSocket_);
    listenSocket_ = -1;
  }
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
  listenSocket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (listenSocket_ < 0) {
    return;
  }

  int opt = 1;
  setsockopt(listenSocket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port_);

  if (bind(listenSocket_, (struct sockaddr*)&address, sizeof(address)) < 0) {
    close(listenSocket_);
    listenSocket_ = -1;
    return;
  }

  if (listen(listenSocket_, 5) < 0) {
    close(listenSocket_);
    listenSocket_ = -1;
    return;
  }

  if (auto log = Core::CLog::GetLogger(Core::log_type::GENERAL).lock()) {
    log->info("Health check listening on port {}", port_);
  }

  while (running_) {
    struct sockaddr_in clientAddr;
    socklen_t clientLen = sizeof(clientAddr);
    int clientSocket = accept(listenSocket_, (struct sockaddr*)&clientAddr, &clientLen);
    if (clientSocket < 0) {
      if (running_) {
        // handle error?
      }
      continue;
    }

    handleRequest(clientSocket);
    close(clientSocket);
  }
}

void HealthServer::handleRequest(int clientSocket) {
  std::array<char, 1024> buffer = {0};
  ssize_t bytesRead = read(clientSocket, buffer.data(), buffer.size() - 1);
  if (bytesRead <= 0) return;
  buffer[bytesRead] = '\0';

  std::string request(buffer.data());
  if (request.find("GET /health") == std::string::npos) {
    std::string response404 = "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n";
    send(clientSocket, response404.c_str(), response404.length(), 0);
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
  send(clientSocket, responseStr.c_str(), responseStr.length(), 0);
}

}  // namespace Core
