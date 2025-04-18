#include "data_generator.hpp"
#include <random>
#include <algorithm>
#include <chrono>

DataGenerator::DataGenerator(double base_price, double price_volatility,
                           int min_quantity, int max_quantity)
    : base_price_(base_price)
    , price_volatility_(price_volatility)
    , min_quantity_(min_quantity)
    , max_quantity_(max_quantity)
    , rng_(std::random_device{}())
    , price_dist_(base_price * (1.0 - price_volatility), 
                  base_price * (1.0 + price_volatility))
    , quantity_dist_(min_quantity, max_quantity)
    , buy_prob_dist_(0.0, 1.0) {}

std::vector<Order> DataGenerator::generate_orders(int num_orders,
                                                std::chrono::nanoseconds start_time,
                                                std::chrono::nanoseconds end_time) {
    std::vector<Order> orders;
    orders.reserve(num_orders);

    for (int i = 0; i < num_orders; ++i) {
        double price = generate_price();
        int quantity = generate_quantity();
        bool is_buy = generate_is_buy();
        std::chrono::nanoseconds timestamp = generate_timestamp(start_time, end_time);

        orders.emplace_back(i + 1, price, quantity, is_buy, timestamp);
    }

    // Sort orders by timestamp
    std::sort(orders.begin(), orders.end(),
              [](const Order& a, const Order& b) {
                  return a.get_timestamp() < b.get_timestamp();
              });

    return orders;
}

double DataGenerator::generate_price() {
    return price_dist_(rng_);
}

int DataGenerator::generate_quantity() {
    return quantity_dist_(rng_);
}

bool DataGenerator::generate_is_buy() {
    return buy_prob_dist_(rng_) < 0.5;
}

std::chrono::nanoseconds DataGenerator::generate_timestamp(
    std::chrono::nanoseconds start_time,
    std::chrono::nanoseconds end_time) {
    
    std::uniform_int_distribution<int64_t> time_dist(
        start_time.count(),
        end_time.count()
    );
    
    return std::chrono::nanoseconds(time_dist(rng_));
} 