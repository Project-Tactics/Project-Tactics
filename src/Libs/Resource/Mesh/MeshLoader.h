#pragma once

#include "Mesh.h"

#include <Libs/Resource/ResourceLoader.h>

namespace tactics::resource {

class MeshLoader : public ResourceLoader {
public:
	using ResourceLoader::ResourceLoader;
	[[nodiscard]] std::shared_ptr<Mesh> load(const json& descriptor);

private:
	static std::vector<float> _parseVertices(const std::string& strVertices);
	static std::vector<unsigned int> _parseIndices(const std::string& strIndices);
	std::shared_ptr<Mesh> _loadMesh(const std::string& path);
	std::shared_ptr<Mesh> _loadMesh(const std::string& strVertices, const std::string& strIndices);
	VertexAttributes _createDefaultVertexAttributes();
};

} // namespace tactics::resource
