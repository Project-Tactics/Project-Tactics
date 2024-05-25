#pragma once

#include <memory>
#include <string_view>
#include <vector>

namespace tactics {
class VertexAttributes;
}

namespace tactics::resource {

class Mesh;

class MeshLoader {
public:
	static std::vector<float> parseVertices(const std::string& strVertices);
	static std::vector<unsigned int> parseIndices(const std::string& strIndices);
	std::unique_ptr<Mesh> loadMesh(std::string_view name, const std::string& path);
	std::unique_ptr<Mesh> loadMesh(std::string_view name, const std::string& strVertices, const std::string& strIndices);

private:
	std::unique_ptr<VertexAttributes> _createDefaultVertexAttributes();
};

}
