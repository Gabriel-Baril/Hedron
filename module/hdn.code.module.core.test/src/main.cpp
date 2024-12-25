#include <iostream>
#include <catch2/catch_all.hpp>

// class CustomReporter : public Catch::EventListenerBase
// {
// public:
// 	using EventListenerBase::EventListenerBase;
// 
// 	void testCaseStarting(const Catch::TestCaseInfo& testInfo) override
// 	{
// 		std::cout << "[TEST_CASE] [" << testInfo.name << "]" << std::endl;
// 	}
// 
// 	void sectionStarting(const Catch::SectionInfo& sectionInfo) override
// 	{
// 		std::cout << " [SECTION] [" << sectionInfo.name << "]" << std::endl;
// 	}
// 
// 	void assertionEnded(const Catch::AssertionStats& assertionInfo)
// 	{
// 		std::cout << "  [ASSERTION]: [" << assertionInfo.assertionResult.getExpressionInMacro() << "] [" << (assertionInfo.assertionResult.succeeded() ? "PASS" : "FAIL") << "]" << std::endl;
// 	}
// 
// 	void testCaseEnded(const Catch::TestCaseStats& testCaseStats) override {
// 		// std::cout << "" << testCaseStats.testInfo->name << std::endl;
// 	}
// };
// 
// CATCH_REGISTER_LISTENER(CustomReporter);

TEST_CASE("Dummy Test Bonjour", "[fast]")
{

	SECTION("Addition Test")
	{
		int a = 1;
		int b = 2;
		REQUIRE(a + b == 3);
	};

	SECTION("Subtraction Test 1")
	{
		REQUIRE(2 - 1 == 1);
	};

	SECTION("Subtraction Test 2")
	{
		// CHECK(1 == 2);
		REQUIRE(2 - 1 == 1);
		// CHECK(2 == 3);
	};

	SECTION("Multiplication Test")
	{
		// CHECK(1 == 2);
		REQUIRE(4 == 4);
		REQUIRE(3 == 3);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(4 == 4);
		REQUIRE(2 * 5 == 9);
	};
	// REQUIRE(2 * 5 == 9);
	// REQUIRE(2 * 5 == 9);
	// REQUIRE(2 * 5 == 9);
};


TEST_CASE("Benchmark Test", "[benchmark]")
{
	int c = 12;
	int d = 14;
	BENCHMARK("Addition Benchmark")
	{
		return c + d;
	};

	BENCHMARK("Multiplication Benchmark")
	{
		return c * d;
	};
};

TEST_CASE("Parametrized Test", "[parametrized]")
{
	int a = GENERATE(1, 2, 3, 4);
	int b = GENERATE(4, 5);
	int expected = a + b;

	REQUIRE((a + b) == expected);
}

TEST_CASE("Null Test Case", "")
{
}