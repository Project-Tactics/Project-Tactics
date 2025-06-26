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

struct QuadBuffers {
	VertexBuffer vertexBuffer;
	IndexBuffer indexBuffer;
	std::unique_ptr<VertexAttributes> vertexAttributes;

	void build() {
		vertexBuffer.setData(
			{
				0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			},
			rp::StaticDraw::value);
		indexBuffer.setData({0, 1, 2, 2, 3, 0}, rp::StaticDraw::value);
		vertexBuffer.bind();

		VertexAttributes::Builder builder;
		builder.attributef(3);
		builder.attributef(2);
		vertexAttributes = builder.create();

		vertexBuffer.unbind();
	}

	void bind() {
		vertexAttributes->bind();
		vertexBuffer.bind();
		indexBuffer.bind();
	}

	void unbind() {
		indexBuffer.unbind();
		vertexBuffer.unbind();
		vertexAttributes->unbind();
	}
};

} // namespace tactics
