#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include "common.hpp"
#include "../concurrency/OrderBuffer.hpp"
#include "../concurrency/LockFreeOrderBuffer.hpp"

class TcpServer {
public:
    TcpServer(LockFreeOrderBuffer& buffer);
    ~TcpServer();

    void run(uint16_t port);
    void shutdown();

    std::optional<int> getSocketForOrder(int orderId);

private:
    void acceptLoop(uint16_t port);
    void handleClient(int client_sock);

    LockFreeOrderBuffer& buffer_;
    std::vector<int> clients_;
    std::unordered_map<int,int> ordersToSockets;
    std::mutex clients_mutex_;
    std::mutex orders_mutex_;
    std::atomic<bool> running_;
    int server_fd_ = -1;
};
