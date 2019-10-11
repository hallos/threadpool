#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "threadpool.h"



TEST_CASE("Thread pool executes function and returns expected value")
{
    thread_pool threadPool(1);

    int value = 6;
    std::function<int()> func = [&value](){ return value; };
    auto future = threadPool.add_work(func);

    future.wait();
    REQUIRE(future.get() == value);
}