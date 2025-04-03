#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include "common.hpp"
#include "../concurrency/OrderBuffer.hpp"

class TcpServer {
public:
    TcpServer(OrderBuffer& buffer);
    ~TcpServer();

    void run(uint16_t port);
    void shutdown();

private:
    void acceptLoop(uint16_t port);
    void handleClient(int client_sock);

    OrderBuffer& buffer_;
    std::vector<int> clients_;
    std::mutex clients_mutex_;
    std::atomic<bool> running_;
    int server_fd_ = -1;
};
