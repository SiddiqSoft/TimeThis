#include "gtest/gtest.h"

#include <thread>
#include <chrono>

#include "../src/TimeThis.hpp"


TEST(examples, Example1)
{
	using namespace std::chrono;
	bool passTest {false};

	try
	{
		// Use initializer list-style instantiation; we do not allow move/assignment construction.
		// Note that the `()` is not required when the lambda/function takes no argument.
		siddiqsoft::TimeThis tt;

		std::this_thread::sleep_for(100ms);
		auto ttx = std::chrono::duration_cast<milliseconds>(tt.elapsed()).count();
		passTest = (ttx > 0);
		// Check ostream operator
		std::cerr << tt << std::endl;

#if defined __cpp_lib_source_location
		// Check the std::formattmer
		std::cerr << std::format("{}\n", tt);
#endif
	}
	catch (...)
	{
		EXPECT_TRUE(false); // if we throw then the test fails.
	}

	// Iff the lambda runs, it should be true
	EXPECT_TRUE(passTest);
}


TEST(examples, Example2)
{
	using namespace std::chrono;
	bool passTest {false};

	try
	{
		// Use initializer list-style instantiation; we do not allow move/assignment construction.
		// Note that the `()` is not required when the lambda/function takes no argument.
		siddiqsoft::TimeThis tt {[&passTest](const auto& delta) { passTest = true; }};

		std::this_thread::sleep_for(100ms);
	}
	catch (...)
	{
		EXPECT_TRUE(false); // if we throw then the test fails.
	}

	// Iff the lambda runs, it should be true
	EXPECT_TRUE(passTest);
}
