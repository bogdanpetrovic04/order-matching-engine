#pragma once

#include "Order.hpp"
#include <vector>

class OrderBook {
public:
    void addOrder(const Order& order);
    size_t getOrderCount() const;

private:
    std::vector<Order> buyOrders_;
    std::vector<Order> sellOrders_;
};
