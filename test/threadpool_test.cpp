#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"
#include "threadpool.h"



TEST_CASE("Thread pool executes function and returns expected value")
{
    hallos::thread_pool threadPool(1);

    int value = 6;

    SECTION("Without arguments")
    {
        std::function<int()> func = [&value](){ return value; };
        auto future = threadPool.add_work(func);
        future.wait();
        REQUIRE(future.get() == value);
    }

    SECTION("With arguments")
    {
        std::function<int(int, int)> func = [](int val1, int val2){ return val1 + val2; };
        auto future = threadPool.add_work(func, value, value);
        future.wait();
        REQUIRE(future.get() == value + value);
    }
}