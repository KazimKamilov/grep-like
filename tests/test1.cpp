#define CATCH_CONFIG_MAIN
#include "catch2/catch_all.hpp"

import runner;

TEST_CASE("test case #1")
{
    {
        const char* argv[] = { "test1", "-j4", "actor", "configs" };
        run_search(sizeof(argv) / sizeof(const char*), argv);
    }

}
