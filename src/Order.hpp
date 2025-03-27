#pragma once

#include <cstdint>

enum class OrderSide {
    BUY,
    SELL
};

struct Order {
    uint64_t id;
    OrderSide side;
    double price;
    uint32_t quantity;
};
