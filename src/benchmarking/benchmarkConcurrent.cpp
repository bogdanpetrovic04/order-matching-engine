
#include "../core/MatchingEngine.hpp"
#include "../concurrency/OrderBuffer.hpp"
#include "../concurrency/LockFreeOrderBuffer.hpp"
#include "../core/Order.hpp"
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>
#include <atomic>
#include <iomanip>
#include <iostream>

const int NUM_THREADS = 10;
const int ORDERS_PER_THREAD = 10000;

void generateOrders(LockFreeOrderBuffer& buffer, int startId) {
    std::default_random_engine rng(std::random_device{}());
    std::uniform_real_distribution<double> priceDist(90.0, 110.0);
    std::uniform_int_distribution<int> qtyDist(1, 100);
    std::bernoulli_distribution sideDist(0.5);

    for (int i = 0; i < ORDERS_PER_THREAD; ++i) {
        Order o;
        o.id = startId + i;
        o.side = sideDist(rng) ? OrderSide::BUY : OrderSide::SELL;
        o.type = OrderType::LIMIT;
        o.price = priceDist(rng);
        o.quantity = qtyDist(rng);
        o.timestamp = static_cast<uint64_t>(std::chrono::system_clock::now().time_since_epoch().count());
        buffer.push(o);
    }
}

int main() {
    LockFreeOrderBuffer buffer;
    MatchingEngine engine;

    std::thread engineThread([&]() {
        engine.run(buffer);
    });

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::thread> workers;
    for (int i = 0; i < NUM_THREADS; ++i) {
        workers.emplace_back(generateOrders, std::ref(buffer), i * ORDERS_PER_THREAD);
    }

    for (auto& t : workers) {
        t.join();
    }

    engine.stop();
    engineThread.join();

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    int totalOrders = NUM_THREADS * ORDERS_PER_THREAD;
    std::cout << "Processed " << totalOrders << " orders in " << elapsed << " ms\n";
    std::cout << "Throughput: " << std::fixed << std::setprecision(0) << (totalOrders * 1000.0 / elapsed) << " orders/sec\n";

    return 0;
}
