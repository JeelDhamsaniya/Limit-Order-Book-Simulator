#pragma once

#include <chrono>
#include <string>

class Order {
public:
    Order(int order_id, double price, int quantity, bool is_buy, 
          std::chrono::nanoseconds timestamp);

    // Getters
    int get_order_id() const { return order_id_; }
    double get_price() const { return price_; }
    int get_quantity() const { return quantity_; }
    bool is_buy() const { return is_buy_; }
    std::chrono::nanoseconds get_timestamp() const { return timestamp_; }

    // Setters
    void set_quantity(int quantity) { quantity_ = quantity; }

    // Utility methods
    std::string to_string() const;
    bool is_valid() const;

private:
    int order_id_;
    double price_;
    int quantity_;
    bool is_buy_;
    std::chrono::nanoseconds timestamp_;
}; 