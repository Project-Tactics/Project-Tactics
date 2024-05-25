#pragma once

#include <vector>
#include <memory>
#include <functional>

namespace tactics {

class VertexAttributes {
public:
	class Builder {
	public:
		void attributef(int count);
		std::unique_ptr<VertexAttributes> create();
		void create(VertexAttributes* outVertexAttribute);
		void defineAttributes(VertexAttributes& vertexAttribute);
	private:
		int _stride{};
		unsigned int _attributeIndex{};
		std::vector<std::function<void()>> _attributes;
	};

	VertexAttributes();
	~VertexAttributes();

	VertexAttributes(const VertexAttributes&) = delete;
	VertexAttributes& operator=(const VertexAttributes&) = delete;

	void bind();
	void unbind();
	void release();

private:
	unsigned int _vao;
};

}
