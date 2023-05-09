/*
    TimeThis : Simple stopwatch implementation with optional callback on destructor
    Version 1.0.0

    https://github.com/SiddiqSoft/TimeThis

    BSD 3-Clause License

    Copyright (c) 2021, Siddiq Software LLC
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

    3. Neither the name of the copyright holder nor the names of its
       contributors may be used to endorse or promote products derived from
       this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once
#ifndef TIMETHIS_HPP
#define TIMETHIS_HPP 1

#include <functional>
#include <chrono>
#include <ostream>
#include <source_location>
#include <format>


namespace siddiqsoft
{
	/// @brief A class to facilitate the execution of lambda when the object is destroyed.
	struct TimeThis
	{
		/// @brief Calculates the duration since the creation of this object
		/// @return Value representing the elapsed duration as timepoint
		[[nodiscard]] auto elapsed() const
		{
			return std::chrono::system_clock::now() - startTimestamp;
		}


		/// @brief When source_location is available, collect the calling location
		explicit TimeThis(const std::source_location& sl = std::source_location::current())
		    : sourceLocation(sl)
		    , startTimestamp(std::chrono::system_clock::now())
		{
		}

		/// @brief Construct an object which holds the callback to be executed upon destruction
		/// @param callback The callback takes timepoint representing the final calculation of the delta
		/// @param context Reference to the context
		explicit TimeThis(std::function<void(const std::chrono::system_clock::duration&)>&& callback,
		                  const std::source_location&                                       sl = std::source_location::current())
		    : sourceLocation(sl)
		    , mCallback(std::move(callback))
		    , startTimestamp(std::chrono::system_clock::now())
		{
		}

		/// @brief Operator for ostream with source_location present
		/// @param os
		/// @param src
		/// @return
		friend std::ostream& operator<<(std::ostream& os, const TimeThis& src)
		{
			os << src.sourceLocation.function_name() << " took " << src.elapsed().count() << "ns";
			return os;
		}

		/// @brief Not supported. Makes no sense to copy another instance as the use-case should allow for a single task per
		/// callback.
		/// @param  ignored
		TimeThis(TimeThis&) = delete;

		/// @brief Not supported. Makes no sense to move another instance as the use-case should allow for a single task per
		/// callback.
		/// @param  ignored
		TimeThis(TimeThis&&) = delete;

		/// @brief Not supported
		/// @param  ignored
		/// @return self
		TimeThis& operator=(TimeThis&) = delete;

		/// @brief Not supported
		/// @param  ignored
		/// @return self
		TimeThis& operator=(TimeThis&&) = delete;

		/// @brief Invoke the callback if present.
		~TimeThis() noexcept
		{
			if (mCallback) mCallback(elapsed());
		}

	private:
		/// @brief The callback
		std::function<void(const std::chrono::system_clock::duration&)> mCallback {};

	public:
		/// @brief The start timestamp
		std::chrono::system_clock::time_point startTimestamp;
		std::source_location sourceLocation;
	}; // struct TimeThis
} // namespace siddiqsoft


/// @brief Formatter for std::format
template <>
struct std::formatter<siddiqsoft::TimeThis> : std::formatter<std::string>
{
	auto format(const siddiqsoft::TimeThis& sv, std::format_context& ctx)
	{
		return std::formatter<std::string>::format(
		        std::format("{} started on {:%FT%T}Z took {}us",
		                    sv.sourceLocation.function_name(),
		                    sv.startTimestamp,
		                    std::chrono::duration_cast<std::chrono::microseconds>(sv.elapsed()).count()),
		        ctx);
	}
};
#endif
