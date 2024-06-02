#include "ScriptingHelper.h"

#include <Libs/Utility/Exception.h>

namespace tactics {

void ScriptingHelper::executeFunction(sol::state_view& luaState, sol::reference& functionReference) {
	sol::function function = luaState.registry()[functionReference.registry_index()];
	if (function.valid()) {
		auto result = function();
		if (!result.valid()) {
			sol::error error = result;
			throw TACTICS_EXCEPTION("Lua Error while executing a function: {}", error.what());
		}
	}
}
int ScriptingHelper::exceptionHandler(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description) {
	// L is the lua state, which you can wrap in a state_view if necessary
	// maybe_exception will contain exception, if it exists
	// description will either be the what() of the exception or a description saying that we hit the general-case catch(...)
	std::cout << "An exception occurred in a function, here's what it says ";
	if (maybe_exception) {
		std::cout << "(straight from the exception): ";
		const std::exception& ex = *maybe_exception;
		std::cout << ex.what() << std::endl;
	} else {
		std::cout << "(from the description parameter): ";
		std::cout.write(description.data(), static_cast<std::streamsize>(description.size()));
		std::cout << std::endl;
	}

	// you must push 1 element onto the stack to be
	// transported through as the error object in Lua
	// note that Lua -- and 99.5% of all Lua users and libraries -- expects a string
	// so we push a single string (in our case, the description of the error)
	return sol::stack::push(L, description);
}

}
