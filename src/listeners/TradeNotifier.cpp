#include "TradeNotifier.hpp"
#include "../networking/common.hpp"

TradeNotifier::TradeNotifier(TcpServer& server)
    : server_(server) {}

void TradeNotifier::onTradeExecuted(const Trade& trade) {
    std::string message = "TRADE," +
                          std::to_string(trade.buyOrderId) + "," +
                          std::to_string(trade.sellOrderId) + "," +
                          std::to_string(trade.price) + "," +
                          std::to_string(trade.quantity);

    auto notify = [&](int orderId) {
        auto maybeSock = server_.getSocketForOrder(orderId);
        if (maybeSock.has_value()) {
            sendMessage(maybeSock.value(), message); 
        }
    };

    notify(trade.buyOrderId);
    notify(trade.sellOrderId);
}
