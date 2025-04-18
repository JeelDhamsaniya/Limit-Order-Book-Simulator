#pragma once

#include <random>
#include <chrono>
#include <vector>
#include "order.hpp"

class DataGenerator {
public:
    DataGenerator(double base_price = 100.0, 
                 double price_volatility = 0.01,
                 int min_quantity = 1,
                 int max_quantity = 1000);

    // Generate synthetic orders
    std::vector<Order> generate_orders(int num_orders, 
                                     std::chrono::nanoseconds start_time,
                                     std::chrono::nanoseconds end_time);

private:
    // Random number generators
    std::mt19937 rng_;
    std::uniform_real_distribution<double> price_dist_;
    std::uniform_int_distribution<int> quantity_dist_;
    std::uniform_real_distribution<double> buy_prob_dist_;

    // Configuration
    double base_price_;
    double price_volatility_;
    int min_quantity_;
    int max_quantity_;

    // Helper methods
    double generate_price();
    int generate_quantity();
    bool generate_is_buy();
    std::chrono::nanoseconds generate_timestamp(std::chrono::nanoseconds start_time,
                                              std::chrono::nanoseconds end_time);
}; 