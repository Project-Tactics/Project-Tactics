#pragma once

#include <cpptrace/cpptrace.hpp>
#include <fmt/core.h>
#include <stacktrace>
#include <stdexcept>
#include <string_view>

namespace tactics {

class Exception : public cpptrace::exception_with_message {
public:
	template<class... Args>
	Exception(fmt::format_string<Args...> formatString, Args&&... args)
		: cpptrace::exception_with_message(fmt::format(formatString, std::forward<Args>(args)...).c_str()) {}
};

} // namespace tactics

#define TACTICS_EXCEPTION(...) tactics::Exception(__VA_ARGS__)
