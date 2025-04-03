#include "OrderSerializer.hpp"
// #include "../core/Order.hpp"
#include <sstream>
#include <vector>
#include <optional>
#include <stdexcept>
#include <iostream>

std::string OrderSerializer::serialize(const Order& order) {
    std::ostringstream oss;
    oss << order.id << ","
        << (order.side == OrderSide::BUY ? "BUY" : "SELL") << ","
        << (order.type == OrderType::LIMIT ? "LIMIT" : "MARKET") << ","
        << order.price << ","
        << order.quantity << ","
        << order.timestamp;
    return oss.str();
}

std::optional<Order> OrderSerializer::deserialize(const std::string& message) {
    std::istringstream iss(message);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(iss, token, ',')) {
        tokens.push_back(token);
    }
    // std::cout<<"msg: " << message << std::endl;
    // std::cout<< tokens.size() << ' ';for (auto u : tokens) std::cout<<u<<' '; std::cout << std::endl;
    if (tokens.size() != 6) return std::nullopt;

    Order order;
    try {
        order.id = std::stoi(tokens[0]);
        order.side = (tokens[1] == "BUY") ? OrderSide::BUY : OrderSide::SELL;
        order.type = (tokens[2] == "LIMIT") ? OrderType::LIMIT : OrderType::MARKET;
        order.price = std::stod(tokens[3]);
        order.quantity = std::stoi(tokens[4]);
        order.timestamp = static_cast<uint64_t>(0);
            // std::chrono::system_clock::now().time_since_epoch().count());
        return order;
    } catch (...) {
        return std::nullopt;
    }
}
