#define CATCH_CONFIG_MAIN
#include "catch2/catch_all.hpp"

import runner;

TEST_CASE("test case #3")
{
    const char* argv[] = { "test3", "wpn", "configs" };
    run_search(sizeof(argv) / sizeof(const char*), argv);
}
