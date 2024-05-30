#pragma once

#include "FileHandle.h"

#include <string>

namespace tactics {

class StringFileHandle: public FileHandle<std::string> {
public:
	StringFileHandle(const std::filesystem::path& path);
	bool exists() const override;
	void save() override;
	void load() override;

private:
	std::filesystem::path _path;
};

}
