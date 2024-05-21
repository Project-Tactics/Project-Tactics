#pragma once

#include <Libs/Resource/Resource.h>
#include <Libs/Rendering/VertexBuffer.h>
#include <Libs/Rendering/IndexBuffer.h>

#include <memory>

namespace tactics {

class Mesh: public Resource<Mesh> {
public:
	static const ResourceType TYPE = ResourceType::Mesh;
	std::unique_ptr<VertexBuffer> vertexBuffer;
	std::unique_ptr<IndexBuffer> indexBuffer;
};

}
