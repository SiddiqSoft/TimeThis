/*
    TimeThis : Simple stopwatch implementation with optional callback on destructor
    Version @PROJECT_VERSION@

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
#include <stdexcept>
#include <type_traits>

#ifndef TIMETHIS_HPP
#define TIMETHIS_HPP 1

#include <functional>
#include <chrono>
#include <ostream>
#include <format>
#include <source_location>

namespace siddiqsoft
{
	/// @brief A class to facilitate the execution of lambda when the object is destroyed.
	struct timethis
	{
		/// @brief Calculates the duration since the creation of this object
		/// @return Value representing the elapsed duration as timepoint
		[[nodiscard]] auto elapsed() const { return std::chrono::system_clock::now() - startTimestamp; }


		/// @brief When source_location is available, collect the calling location
		explicit timethis(const std::source_location& sl = std::source_location::current())
		    : sourceLocation(sl)
		    , startTimestamp(std::chrono::system_clock::now())
		{
		}

		/// @brief Construct an object which holds the callback to be executed upon destruction
		/// @param callback The callback takes timepoint representing the final calculation of the delta
		/// @param context Reference to the context
		explicit timethis(std::function<void(const std::chrono::system_clock::duration&)>&& callback,
		                  const std::source_location&                                       sl = std::source_location::current())
		    : mCallback(std::move(callback))
		    , sourceLocation(sl)
		    , startTimestamp(std::chrono::system_clock::now())
		{
		}

		/// @brief Operator for ostream with source_location present
		/// @param os
		/// @param src
		/// @return
		friend std::ostream& operator<<(std::ostream& os, const timethis& src)
		{
			os << src.sourceLocation.function_name() << " took " << src.elapsed().count() << "ns";
			return os;
		}

		/// @brief Not supported. Makes no sense to copy another instance as the use-case should allow for a single task per
		/// callback.
		/// @param  ignored
		timethis(timethis&) = delete;

		/// @brief Not supported. Makes no sense to move another instance as the use-case should allow for a single task per
		/// callback.
		/// @param  ignored
		timethis(timethis&&) = delete;

		/// @brief Not supported
		/// @param  ignored
		/// @return self
		timethis& operator=(timethis&) = delete;

		/// @brief Not supported
		/// @param  ignored
		/// @return self
		timethis& operator=(timethis&&) = delete;

		/// @brief Invoke the callback if present.
		~timethis() noexcept
		{
			if (mCallback) mCallback(elapsed());
		}

		template <typename charT = char>
		[[nodiscard]] auto to_string() const
		{
			if constexpr (std::is_same<charT, char>()) {
				return std::format<const char*>("{} started on {:%FT%T}Z took {}us",
				                                sourceLocation.function_name(),
				                                startTimestamp,
				                                std::chrono::duration_cast<std::chrono::microseconds>(elapsed()).count());
			}
			else if constexpr (std::is_same<charT, wchar_t>()) {
				throw std::invalid_argument("wchar_t Not implemented");
			}
		}

	private:
		std::string libraryVersionInfo{"@PROJECT_VERSION@"};
		/// @brief The callback
		std::function<void(const std::chrono::system_clock::duration&)> mCallback {};

		/// @brief The location for this scope (saved when instanced.)
		std::source_location sourceLocation;
		/// @brief The start timestamp
		std::chrono::system_clock::time_point startTimestamp;
	}; // struct TimeThis
} // namespace siddiqsoft


template <class charT>
struct std::formatter<siddiqsoft::timethis, charT> : std::formatter<charT>
{
	template <class FC>
	auto format(const siddiqsoft::timethis& sv, FC& ctx) const
	{
		return std::format_to(ctx.out(), "{}", sv.to_string<charT>());
	}
};

#else
#pragma message "Already included timethis.hpp"
#endif
