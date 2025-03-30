find_package(GTest REQUIRED)

add_executable(orderbook_tests
    test_main.cpp
    test_orderbook.cpp
    # test_matchingengine.cpp
    test_orderbuffer.cpp
    ${PROJECT_SOURCE_DIR}/src/core/OrderBook.cpp
    ${PROJECT_SOURCE_DIR}/src/core/MatchingEngine.cpp
    ${PROJECT_SOURCE_DIR}/src/concurrency/OrderBuffer.cpp
)

target_include_directories(orderbook_tests PRIVATE ${PROJECT_SOURCE_DIR}/src)

target_link_libraries(orderbook_tests
    GTest::GTest
    GTest::Main
)

add_test(NAME AllTests COMMAND orderbook_tests)
