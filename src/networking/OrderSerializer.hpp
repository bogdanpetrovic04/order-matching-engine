#pragma once
#include "../core/Order.hpp"
#include <string>

class OrderSerializer {
public:
    static std::string serialize(const Order& order);
    static std::optional<Order> deserialize(const std::string& message);
};
