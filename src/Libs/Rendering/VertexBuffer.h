#pragma once

namespace tactics {

class VertexBuffer {
public:
	VertexBuffer();
	~VertexBuffer();

	void bind();
	void unbind();

	void setData(const void* data, size_t size);

private:
	unsigned int _id;
};

}
