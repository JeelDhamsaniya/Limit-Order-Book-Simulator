#pragma once

#include <map>
#include <vector>
#include <chrono>
#include <string>
#include "order.hpp"

class OrderBook {
public:
    OrderBook();
    ~OrderBook() = default;

    // Core functionality
    bool add_order(const Order& order);
    bool cancel_order(int order_id);
    void match_orders();

    // Getters
    double get_best_bid() const;
    double get_best_ask() const;
    int get_bid_volume() const;
    int get_ask_volume() const;
    double get_spread() const;

    // Statistics
    double get_average_execution_latency() const;
    std::string get_book_state() const;

    // Export functionality
    void export_to_csv(const std::string& filename) const;

private:
    // Order book structure using price-time priority
    std::map<double, std::vector<Order>, std::greater<double>> bids_;  // Descending order for bids
    std::map<double, std::vector<Order>> asks_;  // Ascending order for asks

    // Statistics
    std::vector<std::chrono::nanoseconds> execution_latencies_;
    int total_matches_;

    // Helper methods
    void match_orders_at_price(double price);
    void remove_empty_price_levels();
    bool try_match_orders(Order& bid, Order& ask);
}; 