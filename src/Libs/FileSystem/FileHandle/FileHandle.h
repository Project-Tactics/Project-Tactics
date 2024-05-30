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

template<typename TObject>
class FileHandle: public BaseFileHandle {
public:
	TObject& getObject() {
		return _object;
	}

protected:
	void _setObject(const TObject& object) {
		_object = object;
	}

private:
	TObject _object;
};

}
