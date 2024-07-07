#include "GeometryBuilder.h"

#include <Libs/Resource/Mesh/Mesh.h>

namespace tactics {

GeometryBuilder::GeometryBuilder(std::vector<GeometryBuilder::Attributef> attributes) : _attributes(attributes) {}

void GeometryBuilder::beginSubMesh() {
	_subMeshes.push_back(SubMeshInfo());
	_currentSubMesh = &_subMeshes.back();
}

void GeometryBuilder::endSubMesh() {
	_currentSubMesh = nullptr;
}

void GeometryBuilder::addVertex(std::vector<float> vertex) {
	if (!_currentSubMesh) {
		throw TACTICS_EXCEPTION(
			"GeometryBuilder: Can't addVertex. No SubMesh currently active. Remember to call beginSubMesh.");
	}
	_currentSubMesh->vertices.insert(_currentSubMesh->vertices.end(), vertex.begin(), vertex.end());
}

void GeometryBuilder::addIndex(unsigned int index) {
	if (!_currentSubMesh) {
		throw TACTICS_EXCEPTION(
			"GeometryBuilder: Can't addIndex. No SubMesh currently active. Remember to call beginSubMesh.");
	}
	_currentSubMesh->indices.push_back(index);
}

void GeometryBuilder::addIndices(std::vector<unsigned int> indices) {
	if (!_currentSubMesh) {
		throw TACTICS_EXCEPTION(
			"GeometryBuilder: Can't addIndices. No SubMesh currently active. Remember to call beginSubMesh.");
	}
	_currentSubMesh->indices.insert(_currentSubMesh->indices.end(), indices.begin(), indices.end());
}

std::shared_ptr<resource::Mesh> GeometryBuilder::build(const HashId& name) {
	if (_currentSubMesh) {
		throw TACTICS_EXCEPTION(
			"GeometryBuilder: Can't build mesh. SubMesh is still active. Remember to call endSubMesh.");
	}

	auto mesh = std::make_shared<resource::Mesh>(name);

	for (auto& subMesh : _subMeshes) {
		if (subMesh.vertices.empty()) {
			throw TACTICS_EXCEPTION("GeometryBuilder: Can't build mesh. SubMesh has no vertices.");
		}
		if (subMesh.indices.empty()) {
			throw TACTICS_EXCEPTION("GeometryBuilder: Can't build mesh. SubMesh has no indices.");
		}

		auto vbo = std::make_unique<VertexBuffer>(subMesh.vertices, rp::StaticDraw::value);
		auto ibo = std::make_unique<IndexBuffer>(subMesh.indices, rp::StaticDraw::value);
		vbo->bind();
		auto builder = VertexAttributes::Builder();
		for (auto& attribute : _attributes) {
			builder.attributef(attribute.size);
		}
		auto vao = builder.create();
		vbo->unbind();

		mesh->subMeshes.emplace_back(static_cast<unsigned int>(mesh->subMeshes.size()),
									 std::move(vbo),
									 std::move(ibo),
									 std::move(vao));
	}

	return mesh;
}

} // namespace tactics
