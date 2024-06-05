#define CATCH_CONFIG_MAIN
#include "catch2/catch_all.hpp"

import runner;

TEST_CASE("test case #2-0")
{
    const char* argv[] = { "test2", "-j8", "db", "configs" };
    run_search(sizeof(argv) / sizeof(const char*), argv);
}

TEST_CASE("test case #2-1")
{
    const char* argv[] = { "test2", "-j12", "db", "configs" };
    run_search(sizeof(argv) / sizeof(const char*), argv);
}

TEST_CASE("test case #2-2")
{
    const char* argv[] = { "test2", "-j5", "data", "configs" };
    run_search(sizeof(argv) / sizeof(const char*), argv);
}
