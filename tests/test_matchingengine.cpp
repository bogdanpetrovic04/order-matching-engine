#include <gtest/gtest.h>
#include "core/MatchingEngine.hpp"
/*
// Test when order book is empty: processOrder should add the order and return no trades.
TEST(MatchingEngineTest, NoTradeWhenOrderBookEmpty) {
    MatchingEngine engine;
    Order order1 {1, OrderSide::BUY, OrderType::MARKET, 100.0, 10, 0};  // timestamp set to 0 for now
    std::vector<Trade> trades = engine.processOrder(order1);
    EXPECT_TRUE(trades.empty());
}

// Test when order book is non-empty and a SELL order is processed.
// According to our dummy logic, when processing a SELL order,
// it will create a trade with buyOrderId set to 42.
TEST(MatchingEngineTest, TradeGeneratedForSellOrder) {
    MatchingEngine engine;
    // First, add an order to make the order book non-empty.
    Order order1 {1, OrderSide::BUY, OrderType::MARKET, 100.0, 10, 0};  // timestamp set to 0 for now
    engine.processOrder(order1); // No trade; just adds to order book.
    
    // Now process a SELL order.
    Order order2 {2, OrderSide::SELL, OrderType::MARKET, 99.0, 5, 0};
    std::vector<Trade> trades = engine.processOrder(order2);
    
    // Expect a trade to be generated.
    ASSERT_FALSE(trades.empty());
    EXPECT_EQ(trades.size(), 1);

    Trade trade = trades[0];
    // For a SELL order, our dummy logic sets:
    // trade.buyOrderId = 42 and trade.sellOrderId = order2.id.
    EXPECT_EQ(trade.buyOrderId, order1.id);
    EXPECT_EQ(trade.sellOrderId, order2.id);
    EXPECT_DOUBLE_EQ(trade.price, order1.price);
    // EXPECT_EQ(trade.quantity, std::min(order1.quantity,order2.quantity));
}

TEST(MatchingEngineTest, RealTradeSimulation) {
    MatchingEngine engine;
    // First, add an order to make the order book non-empty.
    Order order1 {1, OrderSide::BUY, OrderType::MARKET, 100.0, 10, 0};  // timestamp set to 0 for now
    engine.processOrder(order1); // No trade; just adds to order book.
    
    // Now process a SELL order.
    Order order2 {2, OrderSide::SELL, OrderType::MARKET, 99.0, 5, 0};
    std::vector<Trade> trades = engine.processOrder(order2);
    
    // Expect a trade to be generated.
    ASSERT_FALSE(trades.empty());
    EXPECT_EQ(trades.size(), 1);

    Trade trade = trades[0];
    EXPECT_EQ(trade.buyOrderId, order1.id);
    EXPECT_EQ(trade.sellOrderId, order2.id);
    EXPECT_DOUBLE_EQ(trade.price, order1.price);
    // EXPECT_EQ(trade.quantity, std::min(order1.quantity, order2.quantity));
}

*/