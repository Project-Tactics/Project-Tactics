#pragma once

namespace tactics {

class IndexBuffer {
public:
	IndexBuffer();
	~IndexBuffer();

	void bind();
	void unbind();

	void setData(const void* data, size_t size);

private:
	unsigned int _id;
};

}
