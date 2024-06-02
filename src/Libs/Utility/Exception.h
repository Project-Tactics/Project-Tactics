#pragma once

#include <fmt/core.h>
#include <stdexcept>
#include <string_view>
#include <stacktrace>

namespace tactics {

class Exception: public std::runtime_error {
public:
	template<class... Args>
	Exception(const std::stacktrace& stackTrace, fmt::format_string<Args...> formatString, Args&&... args)
		: std::runtime_error(fmt::format(formatString, std::forward<Args>(args)...).c_str())
		, _stackTrace(stackTrace) {
	}

	const std::stacktrace& stackTrace() const {
		return _stackTrace;
	}

	std::stacktrace _stackTrace;
};

#define TACTICS_EXCEPTION(...) \
	Exception(std::stacktrace::current(0, 10), __VA_ARGS__)

}
