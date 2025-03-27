# tests/CMakeLists.txt

# Find GTest (you must have it installed via Homebrew)
find_package(GTest REQUIRED)

# Add the test executable
add_executable(orderbook_tests
    test_main.cpp
    # Add more test files here as needed
)

# Link GTest and other required libraries
target_link_libraries(orderbook_tests
    GTest::GTest
    GTest::Main
)

# Register the test with CTest
add_test(NAME OrderBookTests COMMAND orderbook_tests)
