#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include "aabb.hpp"

TEST_CASE("overlap b to right", "[aabb]")
{
    AABB a = {0, 0, 10, 10};
    AABB b = {5, 3, 15, 13};
    Fixed outX = 0, outY = 0;

    calculateOverlap(a, b, outX, outY);

    REQUIRE(outX == 5);
}

TEST_CASE("overlap b to left", "[aabb]")
{
    AABB a = {6, 0, 16, 10};
    AABB b = {0, 0, 10, 10};
    Fixed outX = 0, outY = 0;

    calculateOverlap(a, b, outX, outY);

    REQUIRE(outX == -4);
}

TEST_CASE("no overlap", "[aabb]")
{
    AABB a = {12, 0, 22, 10};
    AABB b = {0, 0, 10, 10};
    Fixed outX = 0, outY = 0;

    calculateOverlap(a, b, outX, outY);

    REQUIRE(outX == 0);
}

TEST_CASE("complete overlap", "[aabb]")
{
    AABB a = {0, 0, 10, 10};
    AABB b = {0, 0, 10, 10};
    Fixed outX = 0, outY = 0;

    calculateOverlap(a, b, outX, outY);

    REQUIRE((outX == 10 || outX == -10));
}

TEST_CASE("test overlap (positive)", "[aabb]")
{
    AABB a = {0, 0, 10, 10};
    AABB b = {5, 0, 15, 10};

    REQUIRE(isOverlap(a, b) == true);
}

TEST_CASE("test overlap (negative)", "[aabb]")
{
    AABB a = {0, 0, 10, 10};
    AABB b = {12, 0, 22, 10};

    REQUIRE(isOverlap(a, b) == false);
}
