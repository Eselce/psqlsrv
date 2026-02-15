
#include "test_dummy.hpp"

TEST_CASE("sum works", "[sum]") {
    REQUIRE(1 + 2 == 3);
    REQUIRE(-1 + 1 == 0);
}

