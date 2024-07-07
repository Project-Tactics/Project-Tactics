#pragma once

#include <filesystem>

namespace tactics {

class BaseFileHandle {
public:
	virtual ~BaseFileHandle() = default;
	virtual bool exists() const = 0;
	virtual void save() = 0;
	virtual void load() = 0;
};

template<typename TContent> class FileHandle : public BaseFileHandle {
public:
	TContent& getContent() {
		return _content;
	}

protected:
	void _setContent(const TContent& content) {
		_content = content;
	}

private:
	TContent _content;
};

} // namespace tactics
