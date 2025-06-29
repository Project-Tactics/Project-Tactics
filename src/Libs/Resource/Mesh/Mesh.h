#pragma once

#include <Libs/Rendering/IndexBuffer.h>
#include <Libs/Rendering/VertexAttributes.h>
#include <Libs/Rendering/VertexBuffer.h>
#include <Libs/Resource/Resource.h>

#include <memory>

namespace tactics::resource {

class SubMesh {
public:
	SubMesh(unsigned int index,
			VertexBuffer&& vertexBuffer,
			IndexBuffer&& indexBuffer,
			VertexAttributes&& vertexAttributes);
	SubMesh(const SubMesh&) = delete;
	SubMesh& operator=(const SubMesh&) = delete;
	SubMesh(SubMesh&&) = default;

	unsigned int getVertexCount() const;
	unsigned int getTrisCount() const;

	const auto& vb() const {
		return _vertexBuffer;
	}

	const auto& ib() const {
		return _indexBuffer;
	}

	const auto& va() const {
		return _vertexAttributes;
	}

	unsigned int getIndex() const {
		return _index;
	}

private:
	unsigned int _index{};
	VertexBuffer _vertexBuffer;
	IndexBuffer _indexBuffer;
	VertexAttributes _vertexAttributes;
};

class Mesh : public Resource<Mesh> {
public:
	static const ResourceType TYPE = ResourceType::Mesh;
	using Resource<Mesh>::Resource;

	unsigned int getTotalVertexCount() const;
	unsigned int getTotalTrisCount() const;

	std::vector<SubMesh> subMeshes;
};

} // namespace tactics::resource
