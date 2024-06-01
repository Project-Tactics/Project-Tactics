#pragma once

#include <vector>
#include <memory>
#include <functional>

namespace tactics {
class VertexBuffer;

class VertexAttributes {
public:
	class Builder {
	public:
		void attributef(int count);
		std::unique_ptr<VertexAttributes> create();
	private:
		void _defineAttributes(VertexAttributes& vertexAttribute);

		unsigned int _stride{};
		unsigned int _attributeIndex{};
		std::vector<std::function<void()>> _attributes;
	};

	VertexAttributes(unsigned int componentPerVertex);
	~VertexAttributes();

	VertexAttributes(const VertexAttributes&) = delete;
	VertexAttributes& operator=(const VertexAttributes&) = delete;

	void bind();
	void unbind();
	void release();
	bool isValid() const;
	unsigned int getVerticesCount(const VertexBuffer& vbo) const;

private:
	unsigned int _vao{};
	unsigned int _componentPerVertex{};
};

}
