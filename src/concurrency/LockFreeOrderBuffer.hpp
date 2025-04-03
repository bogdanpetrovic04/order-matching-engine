#pragma once
#include "../concurrentqueue/concurrentqueue.h"
#include "../core/Order.hpp"


class LockFreeOrderBuffer{
public:
    // multiple producers will be calling push from the network
    void push(Order& order);

    // one consumer, the matching engine run function will pop
    std::optional<Order> pop();          
private:
    moodycamel::ConcurrentQueue<Order> queue_;
};