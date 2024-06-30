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
			std::unique_ptr<VertexBuffer> vertexBuffer,
			std::unique_ptr<IndexBuffer> indexBuffer,
			std::unique_ptr<VertexAttributes> vertexAttributes);
	~SubMesh();
	SubMesh(const SubMesh&) = delete;
	SubMesh& operator=(const SubMesh&) = delete;
	SubMesh(SubMesh&&) = default;

	unsigned int getVertexCount() const;
	unsigned int getTrisCount() const;

	unsigned int index;
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
	std::unique_ptr<VertexAttributes> vertexAttributes;
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
