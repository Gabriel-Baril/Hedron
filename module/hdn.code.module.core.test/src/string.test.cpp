#include <catch2/catch_all.hpp>

TEST_CASE("String Test", "[String]")
{
	SECTION("String Test 1") {
		REQUIRE(1 + 2 == 3);
	}

	SECTION("String Test 2") {
		REQUIRE(2 - 1 == 1);
	}

	SECTION("String Test 3") {
		REQUIRE(2 - 1 == 1);
	}
}