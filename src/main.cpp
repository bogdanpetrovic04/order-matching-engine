#include "core/MatchingEngine.hpp"
#include "concurrency/OrderBuffer.hpp"
#include "core/Order.hpp"
#include "listeners/TradeLogger.hpp"
#include "listeners/TradeNotifier.hpp"
#include "networking/server.hpp"

#include <thread>
#include <vector>
#include <chrono>
#include <random>
#include <iostream>

int main() {
    OrderBuffer buffer;
    MatchingEngine engine;
    TcpServer server(buffer);

    // Add listeners
    TradeLogger logger("trades.csv");
    TradeNotifier notifier;
    engine.addTradeListener(&logger);
    engine.addTradeListener(&notifier);
    
    // Start the matching engine in its own thread (the consumer)
    std::thread matchingThread([&]() {
        engine.run(buffer);  // Infinite loop for now
    });

    // Start the matching engine in its own thread (the producer)
    std::thread serverThread([&]() {
        server.run(9999);  // Infinite loop for now
    });

    // Let the matching engine consume the remaining orders
    std::this_thread::sleep_for(std::chrono::seconds(120));


    server.shutdown();
    serverThread.detach();

    matchingThread.detach();


    return 0;
}
