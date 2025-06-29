#pragma once

#include <Libs/Rendering/IndexBuffer.h>
#include <Libs/Rendering/VertexAttributes.h>
#include <Libs/Rendering/VertexBuffer.h>

#include <memory>

namespace tactics {

/// <summary>
/// QuadBuffers is a structute that holds vb, ib and va for a quad mesh.
/// It's meant to be used for rendering particles or other quad-based geometry.
/// Most probably it's gonna be trashed after I come up with a better approach.
/// </summary>

class QuadBuffers {
public:
	QuadBuffers() {
		_vertexBuffer.generateData(
			{
				0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			},
			rp::StaticDraw::value);
		_indexBuffer.generateData({0, 1, 2, 2, 3, 0}, rp::StaticDraw::value);
		_vertexBuffer.bind();

		VertexAttributes::Builder builder;
		builder.attributef(3);
		builder.attributef(2);
		builder.create(_vertexAttributes);

		_vertexBuffer.unbind();
	}

	QuadBuffers(const QuadBuffers&) = delete;
	QuadBuffers& operator=(const QuadBuffers&) = delete;
	QuadBuffers(QuadBuffers&&) = default;
	QuadBuffers& operator=(QuadBuffers&&) = default;

	const auto& vb() const {
		return _vertexBuffer;
	}

	const auto& ib() const {
		return _indexBuffer;
	}

	const auto& va() const {
		return _vertexAttributes;
	}

	void bind() {
		_vertexAttributes.bind();
		_vertexBuffer.bind();
		_indexBuffer.bind();
	}

	void unbind() {
		_indexBuffer.unbind();
		_vertexBuffer.unbind();
		_vertexAttributes.unbind();
	}

public:
	VertexBuffer _vertexBuffer;
	IndexBuffer _indexBuffer;
	VertexAttributes _vertexAttributes;
};

} // namespace tactics
