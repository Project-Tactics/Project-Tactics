#pragma once

#include <vector>

namespace tactics {

class VertexBuffer {
public:
	VertexBuffer();
	~VertexBuffer();

	void bind();
	void unbind();

	void setData(const std::vector<float>& data);

	unsigned int getSize() const;

private:
	unsigned int _size{};
	unsigned int _id{};
};

}
