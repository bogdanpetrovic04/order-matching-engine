#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include "../core/Order.hpp"

class OrderBuffer{
public:
    // multiple producers will be calling push from the network
    void push(Order& order);

    // one consumer, the matching engine run function will pop
    Order pop();

    // non-blocking consumer for testing purposes
    std::optional<Order> tryPop();              

private:
    std::queue<Order> queue_;
    std::mutex mutex_;
    std::condition_variable cond_;
};