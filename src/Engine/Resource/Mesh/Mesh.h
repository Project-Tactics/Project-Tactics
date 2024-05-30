#pragma once

#include <Libs/Resource/Resource.h>
#include <Libs/Rendering/VertexBuffer.h>
#include <Libs/Rendering/IndexBuffer.h>
#include <Libs/Rendering/VertexAttributes.h>

#include <memory>

namespace tactics::resource {

class Mesh: public Resource<Mesh> {
public:
	static const ResourceType TYPE = ResourceType::Mesh;
	using Resource<Mesh>::Resource;
	Mesh(const std::string& name, std::unique_ptr<VertexBuffer> vertexBuffer, std::unique_ptr<IndexBuffer> indexBuffer, std::unique_ptr<VertexAttributes> vertexAttributes);
	~Mesh();

	unsigned int getVertexCount() const;
	unsigned int getTrisCount() const;

	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
	std::unique_ptr<VertexAttributes> vertexAttributes;
};

}
