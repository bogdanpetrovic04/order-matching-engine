#include "MatchingEngine.hpp"

std::vector<Trade> MatchingEngine::processOrder (const Order& order) {
    std::vector<Trade> trades;
    if ( orderBook_.getOrderCount() > 0 ) {
        Trade trade;
        if ( order.side == OrderSide::BUY ) {
            trade.buyOrderId = order.id;
            trade.sellOrderId = 42;
            trade.price = order.price;
            trade.quantity = order.quantity;
        }
        else {
            trade.buyOrderId = 42;
            trade.sellOrderId = order.id;
            trade.price = order.price;
            trade.quantity = order.quantity;
        }
        trades.push_back(trade);
    }
    else {
        orderBook_.addOrder(order);
    }
    return trades;
}