#include "../core/Trade.hpp"
#include "TradeListener.hpp"
#include "../networking/server.hpp" 
#include "../networking/common.hpp" 

class TradeNotifier : public TradeListener {
public:
    TradeNotifier(TcpServer& server);
    void onTradeExecuted(const Trade& trade) override;

private:
    TcpServer& server_;
};
