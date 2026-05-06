/*
 * TimeThis tests
 * 
 * Tests for the timethis stopwatch implementation
 */

#include <thread>
#include <format>
#include <chrono>
#include <iostream>
#include <sstream>

#include "../include/siddiqsoft/timethis.hpp"
#include "gtest/gtest.h"

namespace {

using namespace std::chrono_literals;

/// Test fixture for timethis tests
class TimeThisTest : public ::testing::Test
{
protected:
	static constexpr auto SLEEP_DURATION = 100ms;
	static constexpr auto TOLERANCE_MS = 50ms;
};

} // anonymous namespace

/// @test Verify that the ostream operator works correctly
TEST_F(TimeThisTest, StreamOperator)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(SLEEP_DURATION);

	// Verify elapsed time is positive
	const auto elapsed_ms = duration_cast<milliseconds>(timer.elapsed()).count();
	EXPECT_GT(elapsed_ms, 0) << "Elapsed time should be positive";

	// Verify stream output works
	std::ostringstream oss;
	oss << timer;
	const auto output = oss.str();
	EXPECT_FALSE(output.empty()) << "Stream output should not be empty";
	EXPECT_NE(output.find("took"), std::string::npos) << "Output should contain 'took'";
}

/// @test Verify that to_string() method works correctly
TEST_F(TimeThisTest, ToStringMethod)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(SLEEP_DURATION);

	const auto output = timer.to_string();
	EXPECT_FALSE(output.empty()) << "to_string() output should not be empty";
	EXPECT_NE(output.find("started on"), std::string::npos) << "Output should contain 'started on'";
	EXPECT_NE(output.find("took"), std::string::npos) << "Output should contain 'took'";
	EXPECT_NE(output.find("us"), std::string::npos) << "Output should contain 'us' (microseconds)";
}

/// @test Verify that std::formatter specialization works correctly
TEST_F(TimeThisTest, StdFormatterSpecialization)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(SLEEP_DURATION);

	const auto elapsed_ms = duration_cast<milliseconds>(timer.elapsed()).count();
	EXPECT_GT(elapsed_ms, 0) << "Elapsed time should be positive";

	// Verify std::format works
	const auto formatted = std::format("{}", timer);
	EXPECT_FALSE(formatted.empty()) << "Formatted output should not be empty";
	EXPECT_NE(formatted.find("started on"), std::string::npos) << "Formatted output should contain 'started on'";
}

/// @test Verify that callback is invoked on destruction
TEST_F(TimeThisTest, CallbackInvocation)
{
	using namespace std::chrono;

	bool callback_invoked = false;
	auto callback = [&callback_invoked](const auto& /* duration */) {
		callback_invoked = true;
	};

	{
		siddiqsoft::timethis timer(std::move(callback));
		EXPECT_FALSE(callback_invoked) << "Callback should not be invoked yet";
	}

	EXPECT_TRUE(callback_invoked) << "Callback should be invoked on destruction";
}

/// @test Verify that callback receives correct duration
TEST_F(TimeThisTest, CallbackReceivesDuration)
{
	using namespace std::chrono;

	bool duration_valid = false;
	auto callback = [&duration_valid](const auto& duration) {
		duration_valid = (duration.count() > 0);
	};

	{
		siddiqsoft::timethis timer(std::move(callback));
		std::this_thread::sleep_for(SLEEP_DURATION);
	}

	EXPECT_TRUE(duration_valid) << "Callback should receive positive duration";
}

/// @test Verify that elapsed() returns positive duration
TEST_F(TimeThisTest, ElapsedReturnsPositiveDuration)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(SLEEP_DURATION);

	const auto elapsed = timer.elapsed();
	EXPECT_GT(elapsed.count(), 0) << "Elapsed duration should be positive";
}

/// @test Verify that copy operations are deleted
TEST_F(TimeThisTest, CopyOperationsDeleted)
{
	// This test verifies at compile-time that copy operations are deleted
	// If this compiles, the test passes
	static_assert(!std::is_copy_constructible_v<siddiqsoft::timethis>,
	              "timethis should not be copy constructible");
	static_assert(!std::is_copy_assignable_v<siddiqsoft::timethis>,
	              "timethis should not be copy assignable");
}

/// @test Verify that move operations are deleted
TEST_F(TimeThisTest, MoveOperationsDeleted)
{
	// This test verifies at compile-time that move operations are deleted
	// If this compiles, the test passes
	static_assert(!std::is_move_constructible_v<siddiqsoft::timethis>,
	              "timethis should not be move constructible");
	static_assert(!std::is_move_assignable_v<siddiqsoft::timethis>,
	              "timethis should not be move assignable");
}
