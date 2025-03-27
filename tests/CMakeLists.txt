find_package(GTest REQUIRED)

add_executable(orderbook_tests
    test_main.cpp
    test_orderbook.cpp
    ${PROJECT_SOURCE_DIR}/src/OrderBook.cpp
)

target_include_directories(orderbook_tests PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(orderbook_tests
    GTest::GTest
    GTest::Main
)

add_test(NAME OrderBookTests COMMAND orderbook_tests)
