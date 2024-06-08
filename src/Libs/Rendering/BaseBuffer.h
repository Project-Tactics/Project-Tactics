#pragma once

#include <Libs/Rendering/RenderCalls/RenderCalls.h>

#include <vector>

namespace tactics {

template<typename T, rp::BufferType BufferType>
class BaseBuffer {
public:
	BaseBuffer() {
		rp::generateBuffers(1, &_id);
	}

	BaseBuffer(const std::vector<T>& data, unsigned int usage) {
		rp::generateBuffers(1, &_id);
		setData(data, usage);
	}

	~BaseBuffer() {
		rp::deleteBuffers(1, &_id);
	}

	BaseBuffer(const BaseBuffer&) = delete;
	BaseBuffer& operator=(const BaseBuffer&) = delete;

	void bind() const {
		rp::bindBuffer(rp::BufferTypeValue<BufferType>::value, _id);
	}

	void unbind() const {
		rp::unbindBuffer(rp::BufferTypeValue<BufferType>::value);
	}

	void release() {
		rp::deleteBuffers(1, &_id);
		_id = 0;
	}

	bool isValid() const {
		return _id != 0;
	}

	void setData(const std::vector<T>& data, unsigned int usage) {
		bind();
		rp::bufferData(rp::BufferTypeValue<BufferType>::value, static_cast<unsigned int>(data.size() * sizeof(T)), data.data(), usage);
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
