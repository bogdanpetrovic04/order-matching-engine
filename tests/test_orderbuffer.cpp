#include <gtest/gtest.h>
#include <thread>
#include "concurrency/OrderBuffer.hpp"

TEST(OrderBufferTest, PushPopSingleThread) {
    OrderBuffer buffer;
    Order order{1, OrderSide::BUY, OrderType::LIMIT, 100.0, 5, 0};
    buffer.push(order);
    Order popped = buffer.pop();

    EXPECT_EQ(popped.id, order.id);
    EXPECT_EQ(popped.price, order.price);
    EXPECT_EQ(popped.quantity, order.quantity);
}

TEST(OrderBufferTest, BlockingPopUnblocksWhenPushed) {
    OrderBuffer buffer;
    std::atomic<bool> popped = false;

    std::thread consumer([&] {
        Order result = buffer.pop();
        popped = true;
    });

    // Give the consumer time to block
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_FALSE(popped);
    Order order = {2, OrderSide::SELL, OrderType::LIMIT, 99.0, 3, 0};
    buffer.push(order);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_TRUE(popped);

    consumer.join();
}

TEST(OrderBufferTest, MultipleProducers) {
    OrderBuffer buffer;
    const int numOrders = 100;
    std::vector<std::thread> producers;

    for (int i = 0; i < numOrders; ++i) {
        producers.emplace_back([&, i] {
            Order order = {(uint64_t)i, OrderSide::BUY, OrderType::LIMIT, 100.0 + i, 1, 0};
            buffer.push(order);
        });
    }

    for (auto& t : producers) {
        t.join();
    }

    std::set<int> seen;
    for (int i = 0; i < numOrders; ++i) {
        Order o = buffer.pop();
        seen.insert(o.id);
    }

    EXPECT_EQ(seen.size(), numOrders);
}
