#include <iostream>
#include <thread>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include "common.hpp"
#include "../core/Order.hpp"
#include "OrderSerializer.hpp"

void reader(int sock) {
    while (true) {
        std::string msg = readMessage(sock);
        if (msg.empty()) break;
        std::cout << "\n[Received] " << msg << "\n> " << std::flush;
    }
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv{};
    serv.sin_family = AF_INET;
    serv.sin_port = htons(9999);
    inet_pton(AF_INET, "127.0.0.1", &serv.sin_addr);

    connect(sock, (sockaddr*)&serv, sizeof(serv));
    std::cout << "Connected to server. Send order:\n";

    std::thread(reader, sock).detach();

    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        if (input == "/quit") break;
        if (!sendMessage(sock, input)) break;
    }

    close(sock);
    return 0;
}
