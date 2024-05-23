#include "MeshLoader.h"

#include "Mesh.h"

#include <Libs/Utilities/Exception.h>

#include <regex>
#include <functional>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

namespace tactics {

template<typename T>
std::vector<T> parseString(const std::string& str, std::function<T(const std::string&)> convertFunc) {
	std::vector<T> result;
	std::regex re("\\s*,\\s*"); // Regular expression to split on commas with optional whitespace around them
	std::sregex_token_iterator it(str.begin(), str.end(), re, -1);
	std::sregex_token_iterator end;

	for (; it != end; ++it) {
		if (!it->str().empty()) {
			result.push_back(convertFunc(it->str()));
		}
	}
	return result;
}

std::vector<float> MeshLoader::parseVertices(const std::string& strVertices) {
	return parseString<float>(strVertices, [] (const std::string& str) { return std::stof(str); });

}

std::vector<unsigned int> MeshLoader::parseIndices(const std::string& strIndices) {
	return parseString<unsigned int>(strIndices, [] (const std::string& str) { return std::stoul(str); });
}

std::unique_ptr<Mesh> MeshLoader::loadMesh(std::string_view name, const std::string& path) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		throw Exception("Failed to load mesh: {}", importer.GetErrorString());
	}

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	// TODO(Gerark) This is a very simple loader, it only loads the first UV channel and the vertices and indices
	// It should be improved to load more data from the mesh like normals, tangents, bitangents, etc.
	for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		const aiMesh* mesh = scene->mMeshes[meshIndex];
		for (unsigned int vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {

			vertices.push_back(mesh->mVertices[vertexIndex].x);
			vertices.push_back(mesh->mVertices[vertexIndex].y);
			vertices.push_back(mesh->mVertices[vertexIndex].z);

			unsigned int uvCount = mesh->GetNumUVChannels();
			for (unsigned int uvIndex = 0; uvIndex < uvCount; ++uvIndex) {
				if (mesh->HasTextureCoords(uvIndex)) {
					aiVector3D& uv = mesh->mTextureCoords[uvIndex][vertexIndex];
					vertices.push_back(uv.x);
					vertices.push_back(uv.y);
				}
			}
		}
		for (unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			const aiFace& face = mesh->mFaces[faceIndex];
			for (unsigned int index = 0; index < face.mNumIndices; ++index) {
				indices.push_back(face.mIndices[index]);
			}
		}
	}

	return std::make_unique<Mesh>(
		name,
		std::make_unique<VertexBuffer>(vertices),
		std::make_unique<IndexBuffer>(indices)
	);
}

}
