#include <gtest/gtest.h>
#include "OrderBook.hpp"

TEST(OrderBookTest, AddSingleBuyOrder) {
    OrderBook book;
    Order order{1, OrderSide::BUY, 100.5, 10};
    book.addOrder(order);
    EXPECT_EQ(book.getOrderCount(), 1);
}

TEST(OrderBookTest, AddBuyAndSellOrders) {
    OrderBook book;
    book.addOrder({1, OrderSide::BUY, 101.0, 5});
    book.addOrder({2, OrderSide::SELL, 102.0, 7});
    EXPECT_EQ(book.getOrderCount(), 2);
}
