#pragma once

#include <sol/sol.hpp>

namespace tactics {

class ScriptingHelper {
public:
	static void executeFunction(sol::state_view& luaState, sol::reference& functionReference);
	static int
	exceptionHandler(lua_State* L, sol::optional<const std::exception&> maybe_exception, sol::string_view description);
};

} // namespace tactics
