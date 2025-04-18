#include "order_book.hpp"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>

OrderBook::OrderBook() : total_matches_(0) {}

bool OrderBook::add_order(const Order& order) {
    if (!order.is_valid()) return false;

    auto start_time = std::chrono::high_resolution_clock::now();
    
    if (order.is_buy()) {
        bids_[order.get_price()].push_back(order);
    } else {
        asks_[order.get_price()].push_back(order);
    }

    match_orders();

    auto end_time = std::chrono::high_resolution_clock::now();
    execution_latencies_.push_back(
        std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time));
    
    return true;
}

bool OrderBook::cancel_order(int order_id) {
    // Search in both bids and asks
    for (auto& [price, orders] : bids_) {
        auto it = std::find_if(orders.begin(), orders.end(),
            [order_id](const Order& order) { return order.get_order_id() == order_id; });
        if (it != orders.end()) {
            orders.erase(it);
            if (orders.empty()) bids_.erase(price);
            return true;
        }
    }

    for (auto& [price, orders] : asks_) {
        auto it = std::find_if(orders.begin(), orders.end(),
            [order_id](const Order& order) { return order.get_order_id() == order_id; });
        if (it != orders.end()) {
            orders.erase(it);
            if (orders.empty()) asks_.erase(price);
            return true;
        }
    }

    return false;
}

void OrderBook::match_orders() {
    while (!bids_.empty() && !asks_.empty()) {
        double best_bid = bids_.begin()->first;
        double best_ask = asks_.begin()->first;

        if (best_bid >= best_ask) {
            match_orders_at_price(best_ask);
        } else {
            break;
        }
    }
    remove_empty_price_levels();
}

void OrderBook::match_orders_at_price(double price) {
    auto& bid_orders = bids_.begin()->second;
    auto& ask_orders = asks_.begin()->second;

    while (!bid_orders.empty() && !ask_orders.empty()) {
        if (!try_match_orders(bid_orders.front(), ask_orders.front())) {
            break;
        }
        total_matches_++;
    }
}

bool OrderBook::try_match_orders(Order& bid, Order& ask) {
    int match_quantity = std::min(bid.get_quantity(), ask.get_quantity());
    
    bid.set_quantity(bid.get_quantity() - match_quantity);
    ask.set_quantity(ask.get_quantity() - match_quantity);

    if (bid.get_quantity() == 0) {
        bids_.begin()->second.erase(bids_.begin()->second.begin());
    }
    if (ask.get_quantity() == 0) {
        asks_.begin()->second.erase(asks_.begin()->second.begin());
    }

    return true;
}

void OrderBook::remove_empty_price_levels() {
    for (auto it = bids_.begin(); it != bids_.end();) {
        if (it->second.empty()) {
            it = bids_.erase(it);
        } else {
            ++it;
        }
    }

    for (auto it = asks_.begin(); it != asks_.end();) {
        if (it->second.empty()) {
            it = asks_.erase(it);
        } else {
            ++it;
        }
    }
}

double OrderBook::get_best_bid() const {
    return bids_.empty() ? 0.0 : bids_.begin()->first;
}

double OrderBook::get_best_ask() const {
    return asks_.empty() ? 0.0 : asks_.begin()->first;
}

int OrderBook::get_bid_volume() const {
    int volume = 0;
    for (const auto& [price, orders] : bids_) {
        for (const auto& order : orders) {
            volume += order.get_quantity();
        }
    }
    return volume;
}

int OrderBook::get_ask_volume() const {
    int volume = 0;
    for (const auto& [price, orders] : asks_) {
        for (const auto& order : orders) {
            volume += order.get_quantity();
        }
    }
    return volume;
}

double OrderBook::get_spread() const {
    if (bids_.empty() || asks_.empty()) return 0.0;
    return get_best_ask() - get_best_bid();
}

double OrderBook::get_average_execution_latency() const {
    if (execution_latencies_.empty()) return 0.0;
    
    double sum = 0.0;
    for (const auto& latency : execution_latencies_) {
        sum += static_cast<double>(latency.count());
    }
    return sum / execution_latencies_.size();
}

std::string OrderBook::get_book_state() const {
    std::stringstream ss;
    ss << "Order Book State:\n";
    ss << "Bids:\n";
    for (auto it = bids_.rbegin(); it != bids_.rend(); ++it) {
        ss << "Price: " << std::fixed << std::setprecision(2) << it->first 
           << " Volume: " << get_bid_volume() << "\n";
    }
    ss << "Asks:\n";
    for (const auto& [price, orders] : asks_) {
        ss << "Price: " << std::fixed << std::setprecision(2) << price 
           << " Volume: " << get_ask_volume() << "\n";
    }
    return ss.str();
}

void OrderBook::export_to_csv(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) return;

    file << "side,price,quantity\n";
    
    // Export bids in descending order
    for (auto it = bids_.rbegin(); it != bids_.rend(); ++it) {
        for (const auto& order : it->second) {
            file << "BID," << std::fixed << std::setprecision(2) << order.get_price() 
                 << "," << order.get_quantity() << "\n";
        }
    }

    // Export asks in ascending order
    for (const auto& [price, orders] : asks_) {
        for (const auto& order : orders) {
            file << "ASK," << std::fixed << std::setprecision(2) << order.get_price() 
                 << "," << order.get_quantity() << "\n";
        }
    }
} 