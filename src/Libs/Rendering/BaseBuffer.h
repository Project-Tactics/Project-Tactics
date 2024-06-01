#pragma once

#include <vector>
#include <glad/gl.h>

namespace tactics {

template<typename T, GLenum BufferType>
class BaseBuffer {
public:
	BaseBuffer() {
		glGenBuffers(1, &_id);
	}

	BaseBuffer(const std::vector<T>& data, GLenum usage = GL_STATIC_DRAW) {
		glGenBuffers(1, &_id);
		setData(data, usage);
	}

	~BaseBuffer() {
		glDeleteBuffers(1, &_id);
	}

	BaseBuffer(const BaseBuffer&) = delete;
	BaseBuffer& operator=(const BaseBuffer&) = delete;

	void bind() const {
		glBindBuffer(BufferType, _id);
	}

	void unbind() const {
		glBindBuffer(BufferType, 0);
	}

	void release() {
		glDeleteBuffers(1, &_id);
		_id = 0;
	}

	bool isValid() const {
		return _id != 0;
	}

	void setData(const std::vector<T>& data, GLenum usage = GL_STATIC_DRAW) {
		bind();
		glBufferData(BufferType, data.size() * sizeof(T), data.data(), usage);
		_size = static_cast<unsigned int>(data.size());
		unbind();
	}

	unsigned int getSize() const {
		return _size;
	}

private:
	unsigned int _size{};
	unsigned int _id{};
};

}
