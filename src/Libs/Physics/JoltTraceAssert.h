#pragma once

#include <Libs/Utility/Exception.h>

#include <Jolt/Jolt.h>
#include <cstdarg>

namespace tactics {

static void joltTrace(const char* format, ...) {
	va_list list;
	va_start(list, format);
	char buffer[1024];
	vsnprintf(buffer, sizeof(buffer), format, list);
	va_end(list);

	LOG_TRACE(Log::Physics, "{}", buffer);
}

// Callback for asserts, connect this to your own assert handler if you have one
static bool joltAssert(const char* inExpression, const char* inMessage, const char* inFile, JPH::uint inLine) {
	TACTICS_EXCEPTION("{}:{}: ({}) {}", inFile, inLine, inExpression, (inMessage != nullptr ? inMessage : ""));
	return true;
};

} // namespace tactics
