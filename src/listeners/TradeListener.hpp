#pragma once

#include "../core/Trade.hpp"

class TradeListener {
public:
    virtual void onTradeExecuted(const Trade& trade) = 0;
    virtual ~TradeListener() = default;

private:
};