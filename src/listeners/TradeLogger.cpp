#include "TradeLogger.hpp"
#include <iostream>

// Constructor
TradeLogger::TradeLogger(const std::string& filename) {
    logFile_.open(filename, std::ios::app);
    if (!logFile_) {
        std::cerr << "Failed to open trade log file: " << filename << std::endl;
    }
}

// Destructor
TradeLogger::~TradeLogger() {
    if (logFile_.is_open()) {
        logFile_.close();
    }
}


void TradeLogger::onTradeExecuted(const Trade& trade) {
    std::lock_guard<std::mutex> lock(logMutex_);

    logFile_ << trade.buyOrderId << ',' << 
    trade.sellOrderId << ',' << 
    trade.price << ',' << 
    trade.quantity << '\n';

    logFile_.flush();
}