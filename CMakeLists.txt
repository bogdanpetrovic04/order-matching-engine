cmake_minimum_required(VERSION 3.10)
project(OrderMatchingEngine LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the main executable
add_executable(order_engine 
    src/main.cpp
    src/core/OrderBook.cpp
    src/core/MatchingEngine.cpp
    src/concurrency/OrderBuffer.cpp
    src/listeners/TradeLogger.cpp
    src/listeners/TradeNotifier.cpp
    src/networking/common.cpp
    src/networking/OrderSerializer.cpp
    src/networking/server.cpp
)

# Add the client executable
add_executable(client 
    src/networking/client.cpp
    src/networking/common.cpp
    src/networking/OrderSerializer.cpp
)

# Enable testing
enable_testing()

# Include tests subdirectory
add_subdirectory(tests)
