#pragma once

#include <functional>
#include <memory>
#include <vector>

namespace tactics {
class VertexBuffer;

class VertexAttributes {
public:
	class Builder {
	public:
		void attributef(int count);
		VertexAttributes create();
		void create(VertexAttributes& va);

	private:
		void _defineAttributes(VertexAttributes& vertexAttribute);

		unsigned int _stride{};
		unsigned int _attributeIndex{};
		std::vector<std::function<void()>> _attributes;
	};

	VertexAttributes();
	~VertexAttributes();

	VertexAttributes(const VertexAttributes&) = delete;
	VertexAttributes& operator=(const VertexAttributes&) = delete;
	VertexAttributes(VertexAttributes&& other) noexcept;
	VertexAttributes& operator=(VertexAttributes&& other) noexcept;

	void bind() const;
	void unbind() const;
	void release();
	bool isValid() const;
	unsigned int getVerticesCount(const VertexBuffer& vbo) const;
	void setComponentPerVertex(unsigned int componentPerVertex);

private:
	unsigned int _vao{};
	unsigned int _componentPerVertex{};
};

} // namespace tactics
