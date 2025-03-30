#include "MatchingEngine.hpp"
#include <iostream>

std::vector<Trade> MatchingEngine::processOrder (Order& order) {
    std::vector<Trade> trades;
    if (order.side == OrderSide::BUY) {
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
    orderBook_.addOrder(order);
    return trades;
}

void MatchingEngine::run(OrderBuffer& buffer) {
    while(true) {
        Order order = buffer.pop();

        std::vector<Trade> trades = processOrder(order);

        for (Trade trade : trades) {
            std::cout << trade.buyOrderId << ' ' << trade.sellOrderId << ' ' << trade.price << ' ' << trade.quantity << std::endl;
        }
    }
}