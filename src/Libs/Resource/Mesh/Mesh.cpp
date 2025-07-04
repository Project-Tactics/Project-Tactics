#include "Mesh.h"

#include <Libs/Utility/Exception.h>

namespace tactics::resource {

unsigned int Mesh::getTotalVertexCount() const {
	unsigned int vertexCount = 0;
	for (const auto& subMesh : subMeshes) {
		vertexCount = subMesh.getVertexCount();
	}
	return vertexCount;
}

unsigned int Mesh::getTotalTrisCount() const {
	unsigned int trisCount = 0;
	for (const auto& subMesh : subMeshes) {
		trisCount = subMesh.getTrisCount();
	}
	return trisCount;
}

SubMesh::SubMesh(unsigned int index, VertexBuffer&& vb, IndexBuffer&& ib, VertexAttributes&& vao)
	: _vertexBuffer(std::move(vb))
	, _indexBuffer(std::move(ib))
	, _vertexAttributes(std::move(vao)) {
	if (_vertexBuffer.getSize() == 0) {
		TACTICS_EXCEPTION("Vertex buffer must not be empty, SubMesh: {}", index);
	}

	if (_indexBuffer.getSize() == 0) {
		TACTICS_EXCEPTION("Index buffer must not be empty, SubMesh: {}", index);
	}

	if (_indexBuffer.getSize() % 3 != 0) {
		TACTICS_EXCEPTION("Index buffer size must be a multiple of 3, SubMesh: {}", index);
	}
}

unsigned int SubMesh::getVertexCount() const {
	return _vertexAttributes.getVerticesCount(_vertexBuffer);
}

unsigned int SubMesh::getTrisCount() const {
	return _indexBuffer.getSize() / 3;
}

} // namespace tactics::resource
