#pragma once

#include <vector>
#include <string_view>
#include <memory>

namespace tactics {

class Mesh;

class MeshLoader {
public:
	static std::vector<float> parseVertices(const std::string& strVertices);
	static std::vector<unsigned int> parseIndices(const std::string& strIndices);
	static std::unique_ptr<Mesh> loadMesh(std::string_view name, const std::string& path);
};

}
