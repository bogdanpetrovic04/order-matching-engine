cmake_minimum_required(VERSION 3.10)
project(OrderMatchingEngine LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the main executable
add_executable(order_engine 
    src/main.cpp
    src/OrderBook.cpp
)

# Enable testing
enable_testing()

# Include tests subdirectory
add_subdirectory(tests)
