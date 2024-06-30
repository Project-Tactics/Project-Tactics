#pragma once

#include "FileHandle.h"

#include <inicpp.h>

namespace tactics {

class IniFileHandle : public FileHandle<ini::IniFile> {
public:
	IniFileHandle(const std::filesystem::path& path);
	~IniFileHandle() override;
	bool exists() const override;
	void save() override;
	void load() override;

private:
	std::filesystem::path _path;
};

} // namespace tactics
