#include "csv_parser.hpp"
#include <sstream>
#include <fstream>
#include <stdexcept>

bool CSVParser::read_orders(const std::string& filename, std::vector<Order>& orders) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    std::string line;
    // Skip header
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (!validate_line(line)) continue;

        Order order(0, 0.0, 0, false, std::chrono::nanoseconds(0));
        if (parse_order_line(line, order)) {
            orders.push_back(order);
        }
    }

    return true;
}

bool CSVParser::write_orders(const std::string& filename, const std::vector<Order>& orders) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    file << "order_id,price,quantity,is_buy,timestamp\n";
    for (const auto& order : orders) {
        file << format_order(order) << "\n";
    }

    return true;
}

bool CSVParser::write_book_state(const std::string& filename,
                               const std::map<double, std::vector<Order>, std::greater<double>>& bids,
                               const std::map<double, std::vector<Order>>& asks) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    file << "side,price,quantity\n";

    // Write bids in descending order
    for (auto it = bids.rbegin(); it != bids.rend(); ++it) {
        for (const auto& order : it->second) {
            file << "BID," << std::fixed << std::setprecision(2) << order.get_price()
                 << "," << order.get_quantity() << "\n";
        }
    }

    // Write asks in ascending order
    for (const auto& [price, orders] : asks) {
        for (const auto& order : orders) {
            file << "ASK," << std::fixed << std::setprecision(2) << order.get_price()
                 << "," << order.get_quantity() << "\n";
        }
    }

    return true;
}

bool CSVParser::parse_order_line(const std::string& line, Order& order) {
    std::stringstream ss(line);
    std::string field;
    std::vector<std::string> fields;

    while (std::getline(ss, field, ',')) {
        fields.push_back(field);
    }

    if (fields.size() != 5) return false;

    try {
        int order_id = std::stoi(fields[0]);
        double price = std::stod(fields[1]);
        int quantity = std::stoi(fields[2]);
        bool is_buy = (fields[3] == "1" || fields[3] == "true");
        std::chrono::nanoseconds timestamp(std::stoll(fields[4]));

        order = Order(order_id, price, quantity, is_buy, timestamp);
        return order.is_valid();
    } catch (const std::exception&) {
        return false;
    }
}

std::string CSVParser::format_order(const Order& order) const {
    std::stringstream ss;
    ss << order.get_order_id() << ","
       << std::fixed << std::setprecision(2) << order.get_price() << ","
       << order.get_quantity() << ","
       << (order.is_buy() ? "1" : "0") << ","
       << order.get_timestamp().count();
    return ss.str();
}

bool CSVParser::validate_line(const std::string& line) const {
    if (line.empty()) return false;
    
    std::stringstream ss(line);
    std::string field;
    int field_count = 0;

    while (std::getline(ss, field, ',')) {
        if (field.empty()) return false;
        field_count++;
    }

    return field_count == 5;
} 