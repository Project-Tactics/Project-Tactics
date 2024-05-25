#pragma once

#include <fmt/core.h>
#include <stdexcept>
#include <string_view>

namespace tactics {

class Exception: public std::runtime_error {
public:
	template<class... Args>
	Exception(fmt::format_string<Args...> formatString, Args&&... args)
		: std::runtime_error(fmt::format(formatString, std::forward<Args>(args)...).c_str()) {
	}
};

}
