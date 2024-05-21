#pragma once

#include <vector>
#include <string_view>

namespace tactics {

class MeshLoader {
public:
	static std::vector<float> parseVertices(const std::string& strVertices);
	static std::vector<unsigned int> parseIndices(const std::string& strIndices);
};

}
