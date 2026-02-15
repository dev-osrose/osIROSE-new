#ifndef _HEALTH_SERVER_H_
#define _HEALTH_SERVER_H_

#include <string>
#include <functional>
#include <atomic>
#include <thread>
#include <vector>
#include <mutex>

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
    void handleRequest(int clientSocket);

    uint16_t port_;
    std::atomic<bool> running_;
    std::atomic<bool> forcedUnhealthy_;
    std::string unhealthyReason_;
    std::mutex reasonMutex_;
    
    std::vector<HealthCheck> checks_;
    std::thread serverThread_;
    int listenSocket_;
};

} // namespace Core

#endif // _HEALTH_SERVER_H_
