#pragma once

#include "FileHandle.h"

#include <nlohmann/json.hpp>

namespace tactics {

class JsonFileHandle: public FileHandle<nlohmann::ordered_json> {
public:
	JsonFileHandle(const std::filesystem::path& path);
	bool exists() const override;
	void save() override;
	void load() override;

private:
	std::filesystem::path _path;
};

}
