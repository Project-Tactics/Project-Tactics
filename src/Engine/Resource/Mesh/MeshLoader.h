#pragma once

#include <Libs/Resource/ResourceLoader.h>
#include "Mesh.h"

namespace tactics::resource {

class MeshLoader: public ResourceLoader {
public:
	std::shared_ptr<Mesh> load(const nlohmann::json& descriptor);

private:
	static std::vector<float> _parseVertices(const std::string& strVertices);
	static std::vector<unsigned int> _parseIndices(const std::string& strIndices);
	std::shared_ptr<Mesh> _loadMesh(const std::string& name, const std::string& path);
	std::shared_ptr<Mesh> _loadMesh(const std::string& name, const std::string& strVertices, const std::string& strIndices);
	std::unique_ptr<VertexAttributes> _createDefaultVertexAttributes();
};

}
