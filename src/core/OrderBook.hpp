#pragma once

#include "Order.hpp"
#include "Trade.hpp"
#include <map>
#include <set>
#include <deque>

class OrderBook {
public:
    void addOrder(const Order& order);
    size_t getBuyOrderCount() const;
    size_t getSellOrderCount() const;
    bool matchBuyOrder(Order& buyOrder, Trade& trade);   
    bool matchSellOrder(Order& buyOrder, Trade& trade);  
    void cancelOrder(uint64_t id);

private:
    std::map<double,std::deque<Order>, std::greater<>> buyOrders_;
    std::map<double,std::deque<Order>> sellOrders_;
    std::set<uint64_t> cancelledOrders;
};
