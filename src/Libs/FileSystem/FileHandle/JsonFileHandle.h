#pragma once

#include "FileHandle.h"

#include <Libs/Utility/Json/Json.h>

namespace tactics {

class PathHelper;

class JsonFileHandle : public FileHandle<ordered_json> {
public:
	JsonFileHandle(const std::filesystem::path& path, PathHelper& pathHelper);
	bool exists() const override;
	void save() override;
	void load() override;

private:
	void _loadFromLua();
	void _loadFromJson();
	void _load(std::istream& stream);

	std::filesystem::path _path;
	PathHelper& _pathHelper;
};

} // namespace tactics
