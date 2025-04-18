#pragma once

#include <string>
#include <vector>
#include <fstream>
#include "order.hpp"

class CSVParser {
public:
    CSVParser() = default;
    ~CSVParser() = default;

    // File operations
    bool read_orders(const std::string& filename, std::vector<Order>& orders);
    bool write_orders(const std::string& filename, const std::vector<Order>& orders);
    bool write_book_state(const std::string& filename, 
                         const std::map<double, std::vector<Order>, std::greater<double>>& bids,
                         const std::map<double, std::vector<Order>>& asks);

private:
    // Helper methods
    bool parse_order_line(const std::string& line, Order& order);
    std::string format_order(const Order& order) const;
    bool validate_line(const std::string& line) const;
}; 