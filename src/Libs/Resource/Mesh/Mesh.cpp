#include "Mesh.h"

#include <Libs/Utility/Exception.h>

namespace tactics::resource {

unsigned int Mesh::getTotalVertexCount() const {
	unsigned int vertexCount = 0;
	for (const auto& subMesh : subMeshes) { vertexCount = subMesh.getVertexCount(); }
	return vertexCount;
}

unsigned int Mesh::getTotalTrisCount() const {
	unsigned int trisCount = 0;
	for (const auto& subMesh : subMeshes) { trisCount = subMesh.getTrisCount(); }
	return trisCount;
}

SubMesh::SubMesh(unsigned int index,
				 std::unique_ptr<VertexBuffer> vb,
				 std::unique_ptr<IndexBuffer> ib,
				 std::unique_ptr<VertexAttributes> vao)
	: vertexBuffer(std::move(vb))
	, indexBuffer(std::move(ib))
	, vertexAttributes(std::move(vao)) {
	if (vertexBuffer->getSize() == 0) {
		throw TACTICS_EXCEPTION("Vertex buffer must not be empty, SubMesh: {}", index);
	}

	if (indexBuffer->getSize() == 0) { throw TACTICS_EXCEPTION("Index buffer must not be empty, SubMesh: {}", index); }

	if (indexBuffer->getSize() % 3 != 0) {
		throw TACTICS_EXCEPTION("Index buffer size must be a multiple of 3, SubMesh: {}", index);
	}
}

SubMesh::~SubMesh() {
	if (indexBuffer) { indexBuffer->release(); }

	if (vertexBuffer) { vertexBuffer->release(); }

	if (vertexAttributes) { vertexAttributes->release(); }
}

unsigned int SubMesh::getVertexCount() const { return vertexAttributes->getVerticesCount(*vertexBuffer); }

unsigned int SubMesh::getTrisCount() const { return indexBuffer->getSize() / 3; }

} // namespace tactics::resource
