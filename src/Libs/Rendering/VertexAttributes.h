#pragma once

#include <functional>
#include <memory>
#include <vector>

namespace tactics {
class VertexBuffer;
class IndexBuffer;
template<typename TVertex> class TVertexBuffer;

class VertexAttributes {
public:
	class Builder {
	public:
		void attributef(int count);
		VertexAttributes create(VertexBuffer& vb, IndexBuffer& ib);
		VertexAttributes create(VertexBuffer& vb);
		void create(VertexAttributes& va, VertexBuffer& vb, IndexBuffer& ib);
		void create(VertexAttributes& va, VertexBuffer& vb);

		template<typename T> void create(VertexAttributes& va, TVertexBuffer<T>& vb) {
			va.setComponentPerVertex(_stride / static_cast<unsigned int>(sizeof(float)));
			va.bind();
			vb.bind();
			_defineAttributes();
			vb.unbind();
			va.unbind();
		}

	private:
		void _defineAttributes();

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
