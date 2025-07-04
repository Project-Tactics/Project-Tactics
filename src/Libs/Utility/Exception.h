#pragma once

#include <Libs/Utility/Log/Log.h>

#include <cassert>
#include <cpptrace/cpptrace.hpp>
#include <fmt/core.h>

namespace tactics {

class Exception {
public:
	template<class... Args>
	static void trigger_exception([[maybe_unused]] const cpptrace::stacktrace& stacktrace,
								  [[maybe_unused]] fmt::format_string<Args...> formatString,
								  [[maybe_unused]] Args&&... args) {
		LOG_EXCEPTION((fmt::format(formatString, std::forward<Args>(args)...)), stacktrace);
		assert(0);
	}
};

} // namespace tactics

#define TACTICS_EXCEPTION(...) tactics::Exception::trigger_exception(cpptrace::generate_trace(), __VA_ARGS__)
