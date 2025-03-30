#include "core/MatchingEngine.hpp"
#include "concurrency/OrderBuffer.hpp"
#include "core/Order.hpp"
#include "listeners/TradeLogger.hpp"
#include "listeners/TradeNotifier.hpp"

#include <thread>
#include <vector>
#include <chrono>
#include <random>
#include <iostream>

void simulateProducers(OrderBuffer& buffer, int producerId, int numOrders) {
    std::default_random_engine gen(producerId);
    std::uniform_real_distribution<double> priceDist(95.0, 105.0);
    std::uniform_int_distribution<int> quantityDist(1, 10);
    std::uniform_int_distribution<int> sideDist(0, 1);

    for (int i = 0; i < numOrders; ++i) {
        OrderSide side = sideDist(gen) == 0 ? OrderSide::BUY : OrderSide::SELL;
        Order order{
            (uint64_t)producerId * 100 + i,        // Unique ID
            side,
            OrderType::LIMIT,
            priceDist(gen),
            (uint32_t)quantityDist(gen),
            static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count())
        };

        buffer.push(order);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));  // simulate delay
    }
}

int main() {
    OrderBuffer buffer;
    MatchingEngine engine;

    // 📝 Add listeners
    TradeLogger logger("trades.csv");
    TradeNotifier notifier;
    engine.addTradeListener(&logger);
    engine.addTradeListener(&notifier);
    
    // Start the matching engine in its own thread (the consumer)
    std::thread matchingThread([&]() {
        engine.run(buffer);  // Infinite loop for now
    });

    // Start multiple producer threads
    const int numProducers = 3;
    const int ordersPerProducer = 10;
    std::vector<std::thread> producers;

    for (int i = 0; i < numProducers; ++i) {
        producers.emplace_back(simulateProducers, std::ref(buffer), i + 1, ordersPerProducer);
    }

    // Join producers after they're done
    for (auto& p : producers) {
        p.join();
    }

    // Let the matching engine consume the remaining orders
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Graceful shutdown
    // engine.stop();
    matchingThread.detach();

    return 0;
}
