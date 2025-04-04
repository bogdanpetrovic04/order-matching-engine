#pragma once

#include "OrderBook.hpp"
#include "Trade.hpp"

void OrderBook::addOrder(const Order& order) {
    if (order.quantity == 0 ) return;
    if (order.side == OrderSide::BUY) {
        buyOrders_[order.price].push_back(order);
    } else {
        sellOrders_[order.price].push_back(order);
    }
}

size_t OrderBook::getBuyOrderCount() const {
    return buyOrders_.size();
}

size_t OrderBook::getSellOrderCount() const {
    return sellOrders_.size();
}

bool OrderBook::matchBuyOrder(Order& order, Trade& trade) {
    if ( getSellOrderCount() == 0 ) return false;
    Order& potentialMatchOrder = sellOrders_.begin()->second.front();
    if ( cancelledOrders.count(potentialMatchOrder.id) ) {
        sellOrders_.begin()->second.pop_front();
        if ( sellOrders_.begin()->second.empty() ) {
            sellOrders_.erase(sellOrders_.begin());
        }
        return matchBuyOrder(order, trade);
    }
    if ( potentialMatchOrder.price > order.price ) {
        return false;
    }
    trade.buyOrderId = order.id;
    trade.sellOrderId = potentialMatchOrder.id;
    trade.price = potentialMatchOrder.price;
    trade.quantity = std::min(order.quantity, potentialMatchOrder.quantity);

    order.quantity -= trade.quantity;
    potentialMatchOrder.quantity -= trade.quantity;

    if ( potentialMatchOrder.quantity == 0 ) {
        sellOrders_.begin()->second.pop_front();
        if ( sellOrders_.begin()->second.empty() ) {
            sellOrders_.erase(sellOrders_.begin());
        }
    }
    return true;
}

bool OrderBook::matchSellOrder(Order& order, Trade& trade) {
    if ( getBuyOrderCount() == 0 ) return false;
    Order& potentialMatchOrder = buyOrders_.begin()->second.front();
    if ( cancelledOrders.count(potentialMatchOrder.id) ) {
        buyOrders_.begin()->second.pop_front();
        if ( buyOrders_.begin()->second.empty() ) {
            buyOrders_.erase(buyOrders_.begin());
        }
        return matchSellOrder(order, trade);
    }
    if ( potentialMatchOrder.price < order.price ) {
        return false;
    }
    trade.sellOrderId = order.id;
    trade.buyOrderId = potentialMatchOrder.id;
    trade.price = potentialMatchOrder.price;
    trade.quantity = std::min(order.quantity, potentialMatchOrder.quantity);

    order.quantity -= trade.quantity;
    potentialMatchOrder.quantity -= trade.quantity;

    if ( potentialMatchOrder.quantity == 0 ) {
        buyOrders_.begin()->second.pop_front();
        if ( buyOrders_.begin()->second.empty() ) {
            buyOrders_.erase(buyOrders_.begin());
        }
    }
    return true;
}

void OrderBook::cancelOrder(uint64_t id) {
    cancelledOrders.insert(id);
}