#pragma once

#include <Libs/Rendering/RenderCalls/RenderCalls.h>

#include <vector>

namespace tactics {

template<typename T, rp::BufferType BufferType> class BaseBuffer {
public:
	BaseBuffer() {
		rp::generateBuffers(1, &_id);
	}

	BaseBuffer(const std::vector<T>& data, unsigned int usage) {
		rp::generateBuffers(1, &_id);
		generateData(data, usage);
	}

	~BaseBuffer() {
		if (_id == 0) {
			return;
		}
		rp::deleteBuffers(1, &_id);
	}

	BaseBuffer(const BaseBuffer&) = delete;
	BaseBuffer& operator=(const BaseBuffer&) = delete;

	BaseBuffer(BaseBuffer&& other) noexcept : _size(other._size), _id(other._id) {
		other._id = 0;
		other._size = 0;
	}

	BaseBuffer& operator=(BaseBuffer&& other) noexcept {
		if (this != &other) {
			release();
			_size = other._size;
			_id = other._id;
			other._id = 0;
			other._size = 0;
		}
		return *this;
	}

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

	void generateData(const std::vector<T>& data, unsigned int usage) {
		bind();
		rp::bufferData(rp::BufferTypeValue<BufferType>::value,
					   static_cast<unsigned int>(data.size() * sizeof(T)),
					   data.data(),
					   usage);
		_size = static_cast<unsigned int>(data.size());
		unbind();
	}

	void updateData(const std::vector<T>& data, unsigned int offset = 0) {
		if (data.empty()) {
			return;
		}
		bind();
		rp::bufferSubData(rp::BufferTypeValue<BufferType>::value,
						  offset * sizeof(T),
						  static_cast<unsigned int>(data.size() * sizeof(T)),
						  data.data());
		unbind();
	}

	unsigned int getSize() const {
		return _size;
	}

private:
	unsigned int _size{};
	unsigned int _id{};
};

} // namespace tactics
