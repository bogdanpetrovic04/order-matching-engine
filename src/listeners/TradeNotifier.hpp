#pragma once
#include "TradeListener.hpp"

class TradeNotifier : public TradeListener {
public:
    void onTradeExecuted(const Trade& trade) override;
};
