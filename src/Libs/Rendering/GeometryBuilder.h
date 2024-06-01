#pragma once

#include "VertexAttributes.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Libs/Utility/Exception.h"

#include <memory>
#include <string_view>
#include <vector>

namespace tactics {
namespace resource {
class Mesh;
}

class GeometryBuilder {
public:
	struct Attributef {
		unsigned int size;
	};

	struct SubMeshInfo {
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
	};

	GeometryBuilder(std::vector<Attributef> attributes);
	void beginSubMesh();
	void addVertex(std::vector<float> vertex);
	void addIndex(unsigned int index);
	void addIndices(std::vector<unsigned int> indices);
	void endSubMesh();

	std::shared_ptr<resource::Mesh> build(std::string_view name);

private:
	std::vector<Attributef> _attributes;
	std::vector<SubMeshInfo> _subMeshes;
	SubMeshInfo* _currentSubMesh{};
};

}
