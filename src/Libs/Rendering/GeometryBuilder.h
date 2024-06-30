#pragma once

#include "IndexBuffer.h"
#include "Libs/Utility/Exception.h"
#include "Libs/Utility/HashId.h"
#include "VertexAttributes.h"
#include "VertexBuffer.h"

#include <memory>
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

	std::shared_ptr<resource::Mesh> build(const HashId& name);

private:
	std::vector<Attributef> _attributes;
	std::vector<SubMeshInfo> _subMeshes;
	SubMeshInfo* _currentSubMesh{};
};

} // namespace tactics
