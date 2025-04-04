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
    if (tokens.size() != 7 && tokens.size() != 2) return std::nullopt;

    if ( tokens.size() == 7 ) {
        Order order;
        try {
            if (tokens[0] != "NEW")
                throw std::runtime_error("Invalid action");
            order.action = OrderAction::NEW;
            order.id = std::stoi(tokens[1]);
            order.side = (tokens[2] == "BUY") ? OrderSide::BUY : OrderSide::SELL;
            order.type = (tokens[3] == "LIMIT") ? OrderType::LIMIT : OrderType::MARKET;
            order.price = std::stod(tokens[4]);
            order.quantity = std::stoi(tokens[5]);
            order.timestamp = static_cast<uint64_t>(0);
                // std::chrono::system_clock::now().time_since_epoch().count());
            return order;
        } catch (...) {
            return std::nullopt;
        }
    }
    else {
        Order order;
        try {
            if (tokens[0] != "CANCEL")
                throw std::runtime_error("Invalid action");
            order.action = OrderAction::CANCEL;
            order.id = std::stoi(tokens[1]);
                // std::chrono::system_clock::now().time_since_epoch().count());
            return order;
        } catch (...) {
            return std::nullopt;
        }
    }

}
