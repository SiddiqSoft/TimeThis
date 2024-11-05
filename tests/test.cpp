/*
 * TimeThis tests
 */

#include <thread>
#include <format>
#include <chrono>
#include <iostream>

#include "../include/siddiqsoft/timethis.hpp"
#include "gtest/gtest.h"


TEST(ImplementationChecks, StreamOperators)
{
	using namespace std::chrono;
	bool passTest {false};

	try {
		// Use initializer list-style instantiation; we do not allow move/assignment construction.
		// Note that the `()` is not required when the lambda/function takes no argument.
		siddiqsoft::timethis tt;

		std::this_thread::sleep_for(100ms);
		auto ttx = std::chrono::duration_cast<milliseconds>(tt.elapsed()).count();
		passTest = (ttx > 0);
		// Check ostream operator
		std::cerr << tt << std::endl;

		std::cerr << tt.to_string() << std::endl;
	}
	catch (...) {
		EXPECT_TRUE(false); // if we throw then the test fails.
	}

	// Iff the lambda runs, it should be true
	EXPECT_TRUE(passTest);
}


TEST(ImplementationChecks, StdFormatterImpl)
{
	using namespace std::chrono;
	bool passTest {false};

	try {
		// Use initializer list-style instantiation; we do not allow move/assignment construction.
		// Note that the `()` is not required when the lambda/function takes no argument.
		siddiqsoft::timethis tt;

		std::this_thread::sleep_for(100ms);
		auto ttx = std::chrono::duration_cast<milliseconds>(tt.elapsed()).count();
		passTest = (ttx > 0);
		
		// Check the std::formattmer
		std::cerr << std::format("{}\n", tt);
	}
	catch (...) {
		EXPECT_TRUE(false); // if we throw then the test fails.
	}

	// Iff the lambda runs, it should be true
	EXPECT_TRUE(passTest);
}


TEST(UsageExamples, WithLambda)
{
	using namespace std::chrono;
	bool passTest {false};

	try {
		// Use initializer list-style instantiation; we do not allow move/assignment construction.
		// Note that the `()` is not required when the lambda/function takes no argument.
		siddiqsoft::timethis tt {[&passTest](const auto& delta) {
			passTest = true;
		}};

		//std::this_thread::sleep_for(100ms);
	}
	catch (...) {
		EXPECT_TRUE(false); // if we throw then the test fails.
	}

	// Iff the lambda runs, it should be true
	EXPECT_TRUE(passTest);
}
