#pragma once

#include <vector>

namespace tactics {

class IndexBuffer {
public:
	IndexBuffer();
	~IndexBuffer();

	void bind();
	void unbind();

	void setData(const std::vector<unsigned int>& data);
	unsigned int getSize() const;

private:
	unsigned int _id{};
	unsigned int _size{};
};

}
