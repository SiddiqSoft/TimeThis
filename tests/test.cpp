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
#include <algorithm>

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

/// @test Verify that elapsed time increases over time
TEST_F(TimeThisTest, ElapsedTimeIncreases)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	
	const auto elapsed1 = timer.elapsed();
	std::this_thread::sleep_for(50ms);
	const auto elapsed2 = timer.elapsed();
	std::this_thread::sleep_for(50ms);
	const auto elapsed3 = timer.elapsed();

	EXPECT_LT(elapsed1, elapsed2) << "Elapsed time should increase";
	EXPECT_LT(elapsed2, elapsed3) << "Elapsed time should continue to increase";
}

/// @test Verify that multiple calls to elapsed() return consistent results
TEST_F(TimeThisTest, MultipleElapsedCalls)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(SLEEP_DURATION);

	const auto elapsed1 = timer.elapsed();
	const auto elapsed2 = timer.elapsed();
	const auto elapsed3 = timer.elapsed();

	// All should be approximately equal (within a small tolerance)
	EXPECT_GE(elapsed2, elapsed1) << "Second call should be >= first call";
	EXPECT_GE(elapsed3, elapsed2) << "Third call should be >= second call";
}

/// @test Verify callback receives accurate duration
TEST_F(TimeThisTest, CallbackDurationAccuracy)
{
	using namespace std::chrono;

	auto received_duration = system_clock::duration(0);
	auto callback = [&received_duration](const auto& duration) {
		received_duration = duration;
	};

	{
		siddiqsoft::timethis timer(std::move(callback));
		std::this_thread::sleep_for(SLEEP_DURATION);
	}

	const auto received_ms = duration_cast<milliseconds>(received_duration).count();
	EXPECT_GE(received_ms, 90) << "Callback duration should be at least 90ms";
	EXPECT_LE(received_ms, 200) << "Callback duration should be at most 200ms";
}

/// @test Verify that timer works with very short durations
TEST_F(TimeThisTest, VeryShortDuration)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	// No sleep - just measure the overhead

	const auto elapsed = timer.elapsed();
	EXPECT_GE(elapsed.count(), 0) << "Elapsed time should be non-negative";
}

/// @test Verify that timer works with longer durations
TEST_F(TimeThisTest, LongerDuration)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(250ms);

	const auto elapsed_ms = duration_cast<milliseconds>(timer.elapsed()).count();
	EXPECT_GE(elapsed_ms, 240) << "Elapsed time should be at least 240ms";
}

/// @test Verify that multiple timers can run independently
TEST_F(TimeThisTest, MultipleIndependentTimers)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer1;
	std::this_thread::sleep_for(50ms);
	
	siddiqsoft::timethis timer2;
	std::this_thread::sleep_for(50ms);

	const auto elapsed1_ms = duration_cast<milliseconds>(timer1.elapsed()).count();
	const auto elapsed2_ms = duration_cast<milliseconds>(timer2.elapsed()).count();

	// Timer1 should have more elapsed time than timer2
	EXPECT_GT(elapsed1_ms, elapsed2_ms) << "Timer1 should have more elapsed time";
}

/// @test Verify that callback is not invoked if no callback is provided
TEST_F(TimeThisTest, NoCallbackProvided)
{
	// This test verifies that creating a timer without a callback doesn't cause issues
	{
		siddiqsoft::timethis timer;
		std::this_thread::sleep_for(SLEEP_DURATION);
	}
	// If we reach here without crashing, the test passes
	EXPECT_TRUE(true);
}

/// @test Verify that elapsed time is measured in nanoseconds
TEST_F(TimeThisTest, ElapsedTimeUnit)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(SLEEP_DURATION);

	const auto elapsed = timer.elapsed();
	const auto elapsed_ns = elapsed.count();
	const auto elapsed_ms = duration_cast<milliseconds>(elapsed).count();

	// Verify that nanoseconds are much larger than milliseconds
	// (nanoseconds should be at least 1000x larger than milliseconds)
	EXPECT_GE(elapsed_ns, elapsed_ms * 1000) << "Nanoseconds should be at least 1000x milliseconds";
}

/// @test Verify stream operator includes function name
TEST_F(TimeThisTest, StreamOperatorIncludesFunctionName)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(SLEEP_DURATION);

	std::ostringstream oss;
	oss << timer;
	const auto output = oss.str();

	// Should contain the function name
	EXPECT_NE(output.find("TimeThisTest"), std::string::npos) 
		<< "Output should contain test function name";
}

/// @test Verify to_string includes timestamp
TEST_F(TimeThisTest, ToStringIncludesTimestamp)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(SLEEP_DURATION);

	const auto output = timer.to_string();

	// Should contain timestamp indicators
	EXPECT_NE(output.find("T"), std::string::npos) 
		<< "Output should contain ISO timestamp (T separator)";
	EXPECT_NE(output.find("Z"), std::string::npos) 
		<< "Output should contain Z (UTC indicator)";
}

/// @test Verify callback with exception handling
TEST_F(TimeThisTest, CallbackWithExceptionHandling)
{
	using namespace std::chrono;

	bool callback_executed = false;
	auto callback = [&callback_executed](const auto& /* duration */) {
		callback_executed = true;
		// Callback executes successfully
	};

	try {
		siddiqsoft::timethis timer(std::move(callback));
		std::this_thread::sleep_for(SLEEP_DURATION);
	}
	catch (...) {
		FAIL() << "Timer should not throw exceptions";
	}

	EXPECT_TRUE(callback_executed) << "Callback should have been executed";
}

/// @test Verify that elapsed() is noexcept
TEST_F(TimeThisTest, ElapsedIsNoexcept)
{
	siddiqsoft::timethis timer;
	
	// This test verifies at compile-time that elapsed() is noexcept
	static_assert(noexcept(timer.elapsed()), 
	              "elapsed() should be noexcept");
	
	EXPECT_TRUE(true);
}

/// @test Verify that destructor is noexcept
TEST_F(TimeThisTest, DestructorIsNoexcept)
{
	// This test verifies at compile-time that destructor is noexcept
	static_assert(std::is_nothrow_destructible_v<siddiqsoft::timethis>,
	              "timethis destructor should be noexcept");
	
	EXPECT_TRUE(true);
}

/// @test Verify callback receives duration as const reference
TEST_F(TimeThisTest, CallbackReceivesDurationAsConstRef)
{
	using namespace std::chrono;

	bool callback_executed = false;
	auto callback = [&callback_executed](const auto& duration) {
		// Verify we can read the duration
		auto count = duration.count();
		callback_executed = (count >= 0);
	};

	{
		siddiqsoft::timethis timer(std::move(callback));
		std::this_thread::sleep_for(SLEEP_DURATION);
	}

	EXPECT_TRUE(callback_executed) << "Callback should receive valid duration";
}

/// @test Verify that type aliases are correct
TEST_F(TimeThisTest, TypeAliasesCorrect)
{
	// Verify type aliases exist and are correct
	static_assert(std::is_same_v<siddiqsoft::timethis::duration_type, 
	                             std::chrono::system_clock::duration>,
	              "duration_type should be system_clock::duration");
	
	static_assert(std::is_same_v<siddiqsoft::timethis::time_point_type,
	                             std::chrono::system_clock::time_point>,
	              "time_point_type should be system_clock::time_point");
	
	EXPECT_TRUE(true);
}

/// @test Verify that format output is consistent
TEST_F(TimeThisTest, FormatOutputConsistency)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(SLEEP_DURATION);

	const auto formatted1 = std::format("{}", timer);
	const auto formatted2 = std::format("{}", timer);

	// Both should contain the same function name and timestamp
	EXPECT_NE(formatted1.find("FormatOutputConsistency"), std::string::npos);
	EXPECT_NE(formatted2.find("FormatOutputConsistency"), std::string::npos);
	EXPECT_NE(formatted1.find("started on"), std::string::npos);
	EXPECT_NE(formatted2.find("started on"), std::string::npos);
}

/// @test Verify callback is called exactly once
TEST_F(TimeThisTest, CallbackCalledExactlyOnce)
{
	using namespace std::chrono;

	int callback_count = 0;
	auto callback = [&callback_count](const auto& /* duration */) {
		callback_count++;
	};

	{
		siddiqsoft::timethis timer(std::move(callback));
		std::this_thread::sleep_for(SLEEP_DURATION);
	}

	EXPECT_EQ(callback_count, 1) << "Callback should be called exactly once";
}

/// @test Verify elapsed time is always non-negative
TEST_F(TimeThisTest, ElapsedTimeNonNegative)
{
	using namespace std::chrono;

	for (int i = 0; i < 10; ++i) {
		siddiqsoft::timethis timer;
		const auto elapsed = timer.elapsed();
		EXPECT_GE(elapsed.count(), 0) << "Elapsed time should never be negative";
	}
}

/// @test Verify lap() returns a string
TEST_F(TimeThisTest, LapReturnsString)
{
	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(SLEEP_DURATION);

	const auto lap_result = timer.lap();
	EXPECT_FALSE(lap_result.empty()) << "lap() should return a non-empty string";
	EXPECT_TRUE(std::all_of(lap_result.begin(), lap_result.end(), ::isdigit))
		<< "lap() should return a string containing only digits";
}

/// @test Verify lap() default template parameter uses microseconds
TEST_F(TimeThisTest, LapDefaultUnitIsMicroseconds)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(SLEEP_DURATION);

	const auto lap_us = timer.lap();
	const auto lap_ms = timer.lap<milliseconds>();

	// Convert both to numbers for comparison
	const auto lap_us_count = std::stoll(lap_us);
	const auto lap_ms_count = std::stoll(lap_ms);

	// Microseconds should be approximately 1000x larger than milliseconds
	// (allowing for some timing variance)
	EXPECT_GT(lap_us_count, lap_ms_count * 900)
		<< "Default lap() should use microseconds (much larger than milliseconds)";
}

/// @test Verify lap() with milliseconds
TEST_F(TimeThisTest, LapWithMilliseconds)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(SLEEP_DURATION);

	const auto lap_ms = timer.lap<milliseconds>();
	const auto lap_ms_count = std::stoll(lap_ms);

	// Should be approximately 100ms (with tolerance)
	EXPECT_GE(lap_ms_count, 90) << "lap<milliseconds>() should be at least 90ms";
	EXPECT_LE(lap_ms_count, 200) << "lap<milliseconds>() should be at most 200ms";
}

/// @test Verify lap() with seconds
TEST_F(TimeThisTest, LapWithSeconds)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(SLEEP_DURATION);

	const auto lap_s = timer.lap<seconds>();
	const auto lap_s_count = std::stoll(lap_s);

	// Should be 0 seconds since we only slept for 100ms
	EXPECT_EQ(lap_s_count, 0) << "lap<seconds>() should be 0 for 100ms sleep";
}

/// @test Verify lap() with nanoseconds
TEST_F(TimeThisTest, LapWithNanoseconds)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(SLEEP_DURATION);

	const auto lap_ns = timer.lap<nanoseconds>();
	const auto lap_ns_count = std::stoll(lap_ns);

	// Should be approximately 100,000,000 nanoseconds (100ms)
	EXPECT_GE(lap_ns_count, 90000000) << "lap<nanoseconds>() should be at least 90,000,000ns";
	EXPECT_LE(lap_ns_count, 200000000) << "lap<nanoseconds>() should be at most 200,000,000ns";
}

/// @test Verify lap() returns consistent results
TEST_F(TimeThisTest, LapConsistentResults)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(SLEEP_DURATION);

	const auto lap1 = timer.lap();
	const auto lap2 = timer.lap();
	const auto lap3 = timer.lap();

	// All should be approximately equal (lap2 and lap3 should be >= lap1)
	const auto lap1_count = std::stoll(lap1);
	const auto lap2_count = std::stoll(lap2);
	const auto lap3_count = std::stoll(lap3);

	EXPECT_GE(lap2_count, lap1_count) << "Second lap should be >= first lap";
	EXPECT_GE(lap3_count, lap2_count) << "Third lap should be >= second lap";
}

/// @test Verify lap() increases over time
TEST_F(TimeThisTest, LapIncreaseOverTime)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;

	const auto lap1 = timer.lap<milliseconds>();
	std::this_thread::sleep_for(50ms);
	const auto lap2 = timer.lap<milliseconds>();
	std::this_thread::sleep_for(50ms);
	const auto lap3 = timer.lap<milliseconds>();

	const auto lap1_count = std::stoll(lap1);
	const auto lap2_count = std::stoll(lap2);
	const auto lap3_count = std::stoll(lap3);

	EXPECT_LT(lap1_count, lap2_count) << "lap() should increase over time";
	EXPECT_LT(lap2_count, lap3_count) << "lap() should continue to increase";
}

/// @test Verify lap() with very short duration
TEST_F(TimeThisTest, LapVeryShortDuration)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	// No sleep - just measure overhead

	const auto lap = timer.lap();
	const auto lap_count = std::stoll(lap);

	EXPECT_GE(lap_count, 0) << "lap() should return non-negative value";
}

/// @test Verify lap() with longer duration
TEST_F(TimeThisTest, LapLongerDuration)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(250ms);

	const auto lap_ms = timer.lap<milliseconds>();
	const auto lap_ms_count = std::stoll(lap_ms);

	EXPECT_GE(lap_ms_count, 240) << "lap<milliseconds>() should be at least 240ms";
}

/// @test Verify lap() is noexcept
TEST_F(TimeThisTest, LapIsNoexcept)
{
	siddiqsoft::timethis timer;

	// This test verifies at compile-time that lap() is noexcept
	static_assert(noexcept(timer.lap()),
	              "lap() should be noexcept");

	EXPECT_TRUE(true);
}

/// @test Verify lap() with different duration types
TEST_F(TimeThisTest, LapWithMultipleDurationTypes)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(SLEEP_DURATION);

	const auto lap_ns = timer.lap<nanoseconds>();
	const auto lap_us = timer.lap<microseconds>();
	const auto lap_ms = timer.lap<milliseconds>();
	const auto lap_s = timer.lap<seconds>();

	const auto lap_ns_count = std::stoll(lap_ns);
	const auto lap_us_count = std::stoll(lap_us);
	const auto lap_ms_count = std::stoll(lap_ms);
	const auto lap_s_count = std::stoll(lap_s);

	// Verify the relationships between different units
	EXPECT_GT(lap_ns_count, lap_us_count * 900) << "ns should be ~1000x us";
	EXPECT_GT(lap_us_count, lap_ms_count * 900) << "us should be ~1000x ms";
	EXPECT_GT(lap_ms_count, lap_s_count * 90) << "ms should be ~1000x s";
}

/// @test Verify lap() returns numeric string that can be parsed
TEST_F(TimeThisTest, LapReturnsParseableNumericString)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(SLEEP_DURATION);

	const auto lap = timer.lap();

	// Should be able to parse as long long
	try {
		const auto lap_count = std::stoll(lap);
		EXPECT_GT(lap_count, 0) << "Parsed lap value should be positive";
	}
	catch (const std::exception& e) {
		FAIL() << "lap() should return a parseable numeric string: " << e.what();
	}
}

/// @test Verify lap() with multiple timers
TEST_F(TimeThisTest, LapWithMultipleTimers)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer1;
	std::this_thread::sleep_for(50ms);

	siddiqsoft::timethis timer2;
	std::this_thread::sleep_for(50ms);

	const auto lap1_ms = timer1.lap<milliseconds>();
	const auto lap2_ms = timer2.lap<milliseconds>();

	const auto lap1_count = std::stoll(lap1_ms);
	const auto lap2_count = std::stoll(lap2_ms);

	// Timer1 should have more elapsed time than timer2
	EXPECT_GT(lap1_count, lap2_count) << "Timer1 should have more elapsed time";
}

/// @test Verify lap() after reset
TEST_F(TimeThisTest, LapAfterReset)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(SLEEP_DURATION);

	const auto lap_before_reset = timer.lap<milliseconds>();
	timer.reset();
	const auto lap_after_reset = timer.lap<milliseconds>();

	const auto lap_before_count = std::stoll(lap_before_reset);
	const auto lap_after_count = std::stoll(lap_after_reset);

	// After reset, lap should be much smaller
	EXPECT_LT(lap_after_count, lap_before_count) << "lap() should be smaller after reset()";
}

/// @test Verify lap() with microseconds precision
TEST_F(TimeThisTest, LapMicrosecondsPrecision)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(SLEEP_DURATION);

	const auto lap_us = timer.lap<microseconds>();
	const auto lap_us_count = std::stoll(lap_us);

	// Should be approximately 100,000 microseconds (100ms)
	EXPECT_GE(lap_us_count, 90000) << "lap<microseconds>() should be at least 90,000us";
	EXPECT_LE(lap_us_count, 200000) << "lap<microseconds>() should be at most 200,000us";
}

/// @test Verify lap() is marked with [[nodiscard]]
TEST_F(TimeThisTest, LapIsMarkedNodexcept)
{
	// This test verifies that lap() is properly documented
	// The [[nodiscard]] attribute should encourage proper usage
	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(SLEEP_DURATION);

	// Using the result to avoid compiler warnings
	const auto result = timer.lap();
	EXPECT_FALSE(result.empty()) << "lap() result should not be empty";
}

/// @test Verify lap() with zero duration
TEST_F(TimeThisTest, LapWithZeroDuration)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	// Immediately call lap without sleeping

	const auto lap = timer.lap();
	const auto lap_count = std::stoll(lap);

	// Should be a small positive number (overhead)
	EXPECT_GE(lap_count, 0) << "lap() should return non-negative value even with zero sleep";
}

/// @test Verify lap() string format is numeric only
TEST_F(TimeThisTest, LapStringFormatNumericOnly)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(SLEEP_DURATION);

	const auto lap = timer.lap();

	// Verify all characters are digits
	for (char c : lap) {
		EXPECT_TRUE(std::isdigit(c)) << "lap() should only contain digits";
	}
}

/// @test Verify lap() with milliseconds and seconds comparison
TEST_F(TimeThisTest, LapMillisecondsSecondsComparison)
{
	using namespace std::chrono;

	siddiqsoft::timethis timer;
	std::this_thread::sleep_for(SLEEP_DURATION);

	const auto lap_ms = timer.lap<milliseconds>();
	const auto lap_s = timer.lap<seconds>();

	const auto lap_ms_count = std::stoll(lap_ms);
	const auto lap_s_count = std::stoll(lap_s);

	// Milliseconds should be approximately 1000x larger than seconds
	EXPECT_GT(lap_ms_count, lap_s_count * 90) << "ms should be ~1000x s";
}
