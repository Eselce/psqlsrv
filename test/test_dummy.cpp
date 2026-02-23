
#include "test_poc.hpp"

#include "test_dummy.hpp"

static test_poc poc;

TEST_CASE("sum works", "[sum]") {
    REQUIRE(1 + 2 == 3);
    REQUIRE(-1 + 1 == 0);
    REQUIRE(poc.run());
}

