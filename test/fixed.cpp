#include <catch2/catch.hpp>

#include "fixed.hpp"

TEST_CASE("addition", "[fixed]")
{
    Fixed a = Fixed::from_raw(384); // 1.5
    Fixed b = Fixed::from_raw(640); // 2.5
    Fixed actual = a + b;
    Fixed expected = Fixed::from_raw(1024); // 4.0

    REQUIRE(actual == expected);
}

TEST_CASE("subtraction", "[fixed]")
{
    Fixed a = Fixed::from_raw(1024); // 4.0
    Fixed b = Fixed::from_raw(384); // 1.5
    Fixed actual = a - b;
    Fixed expected = Fixed::from_raw(640); // 2.5

    REQUIRE(actual == expected);
}

TEST_CASE("multiplication", "[fixed]")
{
    Fixed a = Fixed::from_raw(640); // 2.5
    Fixed b = Fixed::from_raw(384); // 1.5
    Fixed actual = a * b;
    Fixed expected = Fixed::from_raw(960); // 3.75

    REQUIRE(actual == expected);
}

TEST_CASE("division", "[fixed]")
{
    Fixed a = Fixed::from_raw(640); // 2.5
    Fixed b = Fixed::from_raw(384); // 1.5
    Fixed actual = a / b;
    Fixed expected = Fixed::from_raw(426); // 1.66...

    REQUIRE(actual == expected);
}

TEST_CASE("from fraction", "[fixed]")
{
    Fixed a = Fixed::from_fraction(5, 2); // 2.5
    Fixed expected = Fixed::from_raw(640); // 2.5

    REQUIRE(a == expected);
}

TEST_CASE("fraction by division", "[fixed]")
{
    Fixed a = Fixed(5) / 2; // 2.5
    Fixed expected = Fixed::from_fraction(5, 2); // 2.5

    REQUIRE(a == expected);
}


TEST_CASE("literal 2.5", "[fixed]")
{
    Fixed a = "2.5"_f; // 2.5
    Fixed expected = Fixed::from_fraction(25, 10); // 2.5

    REQUIRE(a == expected);
}

TEST_CASE("literal 25.56", "[fixed]")
{
    Fixed a = "25.56"_f; // 25.56
    Fixed expected = Fixed::from_fraction(2556, 100); // 25.56

    REQUIRE(a == expected);
}
