#pragma once

#include <Libs/Resource/Resource.h>
#include <Libs/Rendering/VertexBuffer.h>
#include <Libs/Rendering/IndexBuffer.h>

#include <memory>

namespace tactics {

class Mesh: public Resource<Mesh> {
public:
	static const ResourceType TYPE = ResourceType::Mesh;
	using Resource<Mesh>::Resource;
	Mesh(std::string_view name, std::unique_ptr<VertexBuffer> vertexBuffer, std::unique_ptr<IndexBuffer> indexBuffer);
	Mesh(std::string_view name, std::vector<float> vertexBuffer, std::vector<unsigned int> indexBuffer);
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
};

}
