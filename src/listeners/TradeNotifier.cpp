#include "TradeNotifier.hpp"
#include <iostream>

void TradeNotifier::onTradeExecuted(const Trade& trade) {
    std::cout << "Buyer id: " << trade.buyOrderId << ' ' << 
    "Seller id: " << trade.sellOrderId << ' ' << 
    "Price: " << trade.price << ' ' << 
    "Quantity: " << trade.quantity << '\n';
}