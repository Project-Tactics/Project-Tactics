#include "JsonFileHandle.h"

#include "../PathHelper.h"
#include "StringFileHandle.h"

#include <Libs/Utility/Exception.h>

#include <fstream>
#include <sol/sol.hpp>

namespace tactics {

JsonFileHandle::JsonFileHandle(const std::filesystem::path& path, PathHelper& pathHelper)
	: _path(path)
	, _pathHelper(pathHelper) {
	if (!std::filesystem::exists(_path)) {
		throw TACTICS_EXCEPTION("JsonFileHandle: File [{}] does not exist", _path.string());
	}
	if (std::filesystem::is_empty(_path)) {
		throw TACTICS_EXCEPTION("JsonFileHandle: File [{}] is empty", _path.string());
	}
}

bool JsonFileHandle::exists() const {
	return std::filesystem::exists(_path);
}

void JsonFileHandle::save() {
	if (_path.extension() == ".lua") {
		throw TACTICS_EXCEPTION("Cannot save this json file. Internally it is represented by a lua file. Path: {}",
								_path.string());
	}
	std::ofstream file(_path);
	file << getContent().dump(4);
}

void JsonFileHandle::load() {
	if (_path.extension() == ".lua") {
		_loadFromLua();
	} else {
		_loadFromJson();
	}
}

void JsonFileHandle::_loadFromJson() {
	std::ifstream stream(_path);
	_load(stream);
}

void JsonFileHandle::_loadFromLua() {
	StringFileHandle luaFile(_path);
	luaFile.load();
	std::string content;
	try {
		// TODO(Gerark) Creating a lua state just to load a file is a bit too much. We should refactor this and
		// rely on a ScriptingSystem so we run all the scripts file in a single place.
		sol::state lua;
		lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::table, sol::lib::package, sol::lib::math);
		sol::table packageTable = lua["package"];
		packageTable["path"] = _pathHelper.makeAbsolutePath("?.lua");
		packageTable["cpath"] = "";
		auto result = lua.script(luaFile.getContent());
		if (!result.valid()) {
			sol::error error = result;
			throw TACTICS_EXCEPTION("Failed to load lua file [{}]. Error: {}", _path.string(), error.what());
		}
		auto contentFromLua = result.get<std::optional<std::string>>();
		if (!contentFromLua.has_value()) {
			throw TACTICS_EXCEPTION("Failed to load lua file [{}]. Error: Expected string as return value",
									_path.string());
		}
		content = contentFromLua.value();
	} catch (sol::error& error) {
		throw TACTICS_EXCEPTION("Failed to load lua file [{}]. Error: {}", _path.string(), error.what());
	}

	std::istringstream stream(content);
	_load(stream);
}

void JsonFileHandle::_load(std::istream& stream) {
	try {
		_setContent(nlohmann::ordered_json::parse(stream));
	} catch (nlohmann::json::parse_error& error) {
		throw TACTICS_EXCEPTION("Failed to parse json file [{}]. Error: {}", _path.string(), error.what());
	}
}

} // namespace tactics
