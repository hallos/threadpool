#define CATCH_CONFIG_MAIN
#include "../external/catch2/catch.hpp"
#include "../include/threadpool.h"


TEST_CASE("")
{
    thread_pool threadPool(1);

    std::function<void()> lambda = [](){ std::cout << "HelloWorld!"; };
    threadPool.add_work(lambda);
    REQUIRE(false);

}