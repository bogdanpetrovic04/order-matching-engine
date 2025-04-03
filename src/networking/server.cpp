#include "server.hpp"
#include "OrderSerializer.hpp"
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <algorithm>

TcpServer::TcpServer(OrderBuffer& buffer)
    : buffer_(buffer), running_(true) {}

TcpServer::~TcpServer() {
    shutdown();
}

void TcpServer::run(uint16_t port) {
    std::thread([this, port] {
        acceptLoop(port);
    }).detach();
}

void TcpServer::shutdown() {
    running_ = false;
    if (server_fd_ != -1) {
        close(server_fd_);
    }
    std::lock_guard<std::mutex> lock(clients_mutex_);
    for (int fd : clients_) {
        close(fd);
    }
    clients_.clear();
}

void TcpServer::acceptLoop(uint16_t port) {
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd_, (sockaddr*)&addr, sizeof(addr));
    listen(server_fd_, 10);
    std::cout << "Server listening on port " << port << "...\n";

    while (running_) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_sock = accept(server_fd_, (sockaddr*)&client_addr, &client_len);
        if (client_sock < 0) continue;

        {
            std::lock_guard<std::mutex> lock(clients_mutex_);
            std::cout<< "Client connected on " << client_sock << std::endl;
            clients_.push_back(client_sock);
        }

        std::thread(&TcpServer::handleClient, this, client_sock).detach();
    }
}

void TcpServer::handleClient(int client_sock) {
    while (running_) {
        std::string msg = readMessage(client_sock);
        if (msg.empty()) break;

        auto maybeOrder = OrderSerializer::deserialize(msg);
        if (maybeOrder.has_value()) {
            {
                std::lock_guard<std::mutex> lock(orders_mutex_);
                ordersToSockets[maybeOrder.value().id] = client_sock;
            }
            buffer_.push(maybeOrder.value());
        } else {
            std::cerr << "[Invalid Order] " << msg << "\n";
        }
    }

    close(client_sock);
    {
        std::lock_guard<std::mutex> lock(clients_mutex_);
        std::cout<< "Client disconnected on " << client_sock << std::endl;
        clients_.erase(std::remove(clients_.begin(), clients_.end(), client_sock), clients_.end());
    }
}

std::optional<int> TcpServer::getSocketForOrder(int orderId) {
    std::lock_guard<std::mutex> lock(orders_mutex_);
    auto it = ordersToSockets.find(orderId);
    if (it != ordersToSockets.end()) return it->second;
    return std::nullopt;
}
