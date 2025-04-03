#include "core/MatchingEngine.hpp"
#include "concurrency/OrderBuffer.hpp"
#include "core/Order.hpp"
#include "listeners/TradeLogger.hpp"
#include "listeners/TradeNotifier.hpp"
#include "networking/server.hpp"

#include <thread>
#include <chrono>
#include <iostream>
#include <csignal>
#include <atomic>

// Global flag for Ctrl+C handling
std::atomic<bool> running(true);

// Signal handler to catch Ctrl+C
void signalHandler(int signum) {
    std::cout << "\n[Signal] Caught signal " << signum << ", initiating shutdown...\n";
    running = false;
}

int main() {
    // Register signal handler
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    // Core components
    OrderBuffer buffer;
    MatchingEngine engine;
    TcpServer server(buffer);

    // Attach listeners
    TradeLogger logger("trades.csv");
    TradeNotifier notifier(server);
    engine.addTradeListener(&logger);
    engine.addTradeListener(&notifier);

    // Launch Matching Engine thread
    std::thread matchingThread([&]() {
        engine.run(buffer);
    });

    // Launch TCP Server thread
    std::thread serverThread([&]() {
        server.run(9999);  // Listening on port 9999
    });

    // Main loop waits for Ctrl+C
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Begin shutdown
    std::cout << "[Main] Shutting down components...\n";
    server.shutdown();
    engine.stop();

    // Wait for threads to finish
    serverThread.join();
    matchingThread.join();

    std::cout << "[Main] Shutdown complete.\n";
    return 0;
}
