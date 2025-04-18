#include "order.hpp"
#include <sstream>
#include <iomanip>

Order::Order(int order_id, double price, int quantity, bool is_buy, 
             std::chrono::nanoseconds timestamp)
    : order_id_(order_id)
    , price_(price)
    , quantity_(quantity)
    , is_buy_(is_buy)
    , timestamp_(timestamp) {}

std::string Order::to_string() const {
    std::stringstream ss;
    ss << "Order[id=" << order_id_
       << ", price=" << std::fixed << std::setprecision(2) << price_
       << ", quantity=" << quantity_
       << ", " << (is_buy_ ? "BUY" : "SELL")
       << ", timestamp=" << timestamp_.count() << "ns]";
    return ss.str();
}

bool Order::is_valid() const {
    return order_id_ > 0 && price_ > 0.0 && quantity_ > 0;
} 