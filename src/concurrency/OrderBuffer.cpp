#include "OrderBuffer.hpp"

void OrderBuffer::push(Order& order) {
    // vise producera pushuje u queue
    // potrebno je lock aquire za push, neka mutex varijanta
    // signalovanje popu preko notify_one() ??

    {
        // uzima mutual exclusion lock za buffer push 
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(order);
    }

    // obavestava pop da ima nesto u queue
    cond_.notify_one(); 
}

Order OrderBuffer::pop() {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait(lock, [this] { return !queue_.empty(); }); //wait till queue not empty
    Order order = queue_.front();
    queue_.pop();
    return order;
}