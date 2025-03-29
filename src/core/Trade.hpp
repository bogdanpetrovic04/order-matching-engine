#pragma once

#include <cstdint>

struct Trade {
    uint64_t buyOrderId;
    uint64_t sellOrderId;
    double price;
    uint32_t quantity;
};