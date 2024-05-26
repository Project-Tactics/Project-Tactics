#pragma once

#include <Libs/Resource/ResourceManager.h>
#include "Mesh.h"

namespace tactics::resource {

class MeshManager: public TResourceManager<Mesh> {
public:
	using TResourceManager<Mesh>::TResourceManager;
	ResourceId load(const nlohmann::json& descriptor) override;

private:
	static std::vector<float> _parseVertices(const std::string& strVertices);
	static std::vector<unsigned int> _parseIndices(const std::string& strIndices);
	std::unique_ptr<Mesh> _loadMesh(const std::string& name, const std::string& path);
	std::unique_ptr<Mesh> _loadMesh(const std::string& name, const std::string& strVertices, const std::string& strIndices);
	std::unique_ptr<VertexAttributes> _createDefaultVertexAttributes();
};

}
