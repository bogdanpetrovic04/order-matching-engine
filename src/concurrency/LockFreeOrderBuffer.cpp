#include "LockFreeOrderBuffer.hpp"

void LockFreeOrderBuffer::push(Order& order) {
    queue_.enqueue(order);
}


std::optional<Order> LockFreeOrderBuffer::pop() {
    Order order;
    if ( queue_.try_dequeue(order) ) {
        return order;
    }
    return std::nullopt;
}