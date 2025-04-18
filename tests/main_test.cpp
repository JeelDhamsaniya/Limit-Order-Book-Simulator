#include <gtest/gtest.h>
#include <chrono>
#include "order_book.hpp"

class OrderBookTest : public ::testing::Test {
protected:
    OrderBook book;
    std::chrono::nanoseconds timestamp{0};
};

TEST_F(OrderBookTest, AddBuyOrder) {
    Order order(1, 100.0, 10, true, timestamp);
    EXPECT_TRUE(book.add_order(order));
    EXPECT_DOUBLE_EQ(book.get_best_bid(), 100.0);
    EXPECT_DOUBLE_EQ(book.get_best_ask(), 0.0);
    EXPECT_EQ(book.get_bid_volume(), 10);
    EXPECT_EQ(book.get_ask_volume(), 0);
}

TEST_F(OrderBookTest, AddSellOrder) {
    Order order(1, 100.0, 10, false, timestamp);
    EXPECT_TRUE(book.add_order(order));
    EXPECT_DOUBLE_EQ(book.get_best_bid(), 0.0);
    EXPECT_DOUBLE_EQ(book.get_best_ask(), 100.0);
    EXPECT_EQ(book.get_bid_volume(), 0);
    EXPECT_EQ(book.get_ask_volume(), 10);
}

TEST_F(OrderBookTest, MatchOrders) {
    // Add a buy order
    Order buy_order(1, 100.0, 10, true, timestamp);
    EXPECT_TRUE(book.add_order(buy_order));

    // Add a matching sell order
    Order sell_order(2, 100.0, 5, false, timestamp);
    EXPECT_TRUE(book.add_order(sell_order));

    // Check that orders were matched
    EXPECT_EQ(book.get_bid_volume(), 5);  // Remaining buy volume
    EXPECT_EQ(book.get_ask_volume(), 0);  // All sell volume matched
}

TEST_F(OrderBookTest, PriceTimePriority) {
    // Add multiple orders at different prices
    Order buy1(1, 100.0, 10, true, timestamp);
    Order buy2(2, 99.0, 10, true, timestamp);
    Order sell1(3, 101.0, 10, false, timestamp);
    Order sell2(4, 102.0, 10, false, timestamp);

    EXPECT_TRUE(book.add_order(buy1));
    EXPECT_TRUE(book.add_order(buy2));
    EXPECT_TRUE(book.add_order(sell1));
    EXPECT_TRUE(book.add_order(sell2));

    // Check price-time priority
    EXPECT_DOUBLE_EQ(book.get_best_bid(), 100.0);
    EXPECT_DOUBLE_EQ(book.get_best_ask(), 101.0);
}

TEST_F(OrderBookTest, CancelOrder) {
    // Add an order
    Order order(1, 100.0, 10, true, timestamp);
    EXPECT_TRUE(book.add_order(order));

    // Cancel the order
    EXPECT_TRUE(book.cancel_order(1));
    EXPECT_EQ(book.get_bid_volume(), 0);
}

TEST_F(OrderBookTest, InvalidOrder) {
    // Test with invalid price
    Order order(1, -100.0, 10, true, timestamp);
    EXPECT_FALSE(book.add_order(order));

    // Test with invalid quantity
    Order order2(2, 100.0, 0, true, timestamp);
    EXPECT_FALSE(book.add_order(order2));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 