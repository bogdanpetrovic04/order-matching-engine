#include "OrderBook.hpp"

void OrderBook::addOrder(const Order& order) {
    if (order.side == OrderSide::BUY) {
        buyOrders_.push_back(order);
    } else {
        sellOrders_.push_back(order);
    }
}

size_t OrderBook::getOrderCount() const {
    return buyOrders_.size() + sellOrders_.size();
}
