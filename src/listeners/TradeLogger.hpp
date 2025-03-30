#pragma once
#include "TradeListener.hpp"
#include <fstream>
#include <mutex>
#include <string>

class TradeLogger : public TradeListener {
public:
    TradeLogger(const std::string& filename);
    ~TradeLogger();

    void onTradeExecuted(const Trade& trade) override;

private:
    std::ofstream logFile_;
    std::mutex logMutex_;
};
