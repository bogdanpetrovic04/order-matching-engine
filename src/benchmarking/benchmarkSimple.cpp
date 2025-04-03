#include "../core/MatchingEngine.hpp"
#include "../concurrency/OrderBuffer.hpp"
#include "../core/Order.hpp"

#include <thread>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <random>

int main() {
    const int NUM_ORDERS = 100000000;

    OrderBuffer buffer;
    MatchingEngine engine;

    // No listeners — we're just benchmarking the core

    std::thread engineThread([&]() {
        engine.run(buffer);
    });

    auto start = std::chrono::high_resolution_clock::now();

    // Generate and push random orders
    std::default_random_engine rng;
    std::uniform_real_distribution<double> priceDist(90.0, 110.0);
    std::uniform_int_distribution<int> qtyDist(1, 100);
    std::bernoulli_distribution sideDist(0.5);

    for (int i = 0; i < NUM_ORDERS; ++i) {
        Order o;
        o.id = i;
        o.side = sideDist(rng) ? OrderSide::BUY : OrderSide::SELL;
        o.type = OrderType::LIMIT;
        o.price = priceDist(rng);
        o.quantity = qtyDist(rng);
        o.timestamp = static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count());

        buffer.push(o);
    }

    engine.stop();
    engineThread.join();

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Processed " << NUM_ORDERS << " orders in " << elapsed << " ms\n";
    std::cout << "Throughput: " << std::fixed << std::setprecision(0) << (NUM_ORDERS * 1000.0 / elapsed) << " orders/sec\n";
}
