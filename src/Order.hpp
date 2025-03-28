#pragma once

#include <cstdint>

using Timestamp = uint64_t;  // milliseconds since epoch

enum class OrderSide {
    BUY,
    SELL
};

enum class OrderType {
    LIMIT,
    MARKET
};

struct Order {
    uint64_t id;
    OrderSide side;
    OrderType type;
    double price;
    uint32_t quantity;
    Timestamp timestamp;
};
