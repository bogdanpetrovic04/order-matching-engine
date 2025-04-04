#include "MatchingEngine.hpp"
#include <iostream>

std::vector<Trade> MatchingEngine::processOrder (Order& order) {
    std::vector<Trade> trades;
    if (order.side == OrderSide::BUY) {
        if (order.type == OrderType::MARKET) order.price = std::numeric_limits<double>::max();
        while (1) {
            Trade trade;
            if ( orderBook_.matchBuyOrder(order, trade) ) {
                trades.push_back(trade);
                if ( order.quantity == 0 ) {
                    break;
                }
            }
            else break;
        }
    } 
    else {
        if (order.type == OrderType::MARKET) order.price = -1;
        while (1) {
            Trade trade;
            if ( orderBook_.matchSellOrder(order, trade) ) {
                trades.push_back(trade);
                if ( order.quantity == 0 ) {
                    break;
                }
            }
            else break;
        }
    }
    if ( order.type == OrderType::LIMIT)
        orderBook_.addOrder(order);
    return trades;
}

void MatchingEngine::run(LockFreeOrderBuffer& buffer) {
    while(running_) {
        auto maybeOrder = buffer.pop();
        std::vector<Trade> trades;
        if (maybeOrder.has_value()) {
            if ( maybeOrder.value().action == OrderAction::CANCEL) {
                cancelOrder(maybeOrder.value().id);
            }
            else {
                trades = processOrder(maybeOrder.value());
            }
        }

        for (Trade trade : trades) {
            notifyListeners(trade);
        }
    }
}

void MatchingEngine::cancelOrder(uint64_t id) {
    orderBook_.cancelOrder(id);
}

void MatchingEngine::stop() {
    running_ = false;
}

void MatchingEngine::addTradeListener(TradeListener* listener) {
    listeners_.push_back(listener);
}

void MatchingEngine::notifyListeners(const Trade& trade) {
    for (auto listener : listeners_) {
        listener->onTradeExecuted(trade);
    }
}