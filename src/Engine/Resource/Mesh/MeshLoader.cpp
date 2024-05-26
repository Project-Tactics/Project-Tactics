#include "MeshLoader.h"

#include <Libs/Resource/ResourcePathHelper.h>
#include <Libs/Utility/Exception.h>

#include <regex>
#include <functional>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

namespace tactics::resource {

struct MeshInlineDescriptor {
	std::string name;
	std::string vertices;
	std::string indices;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(MeshInlineDescriptor, name, vertices, indices);
};

std::unique_ptr<Mesh> MeshLoader::load(const nlohmann::json& descriptor) {
	std::unique_ptr<Mesh> mesh;
	if (!descriptor.contains("path")) {
		auto meshDescriptor = descriptor.template get<MeshInlineDescriptor>();
		mesh = _loadMesh(meshDescriptor.name, meshDescriptor.vertices, meshDescriptor.indices);
	} else {
		auto meshDescriptor = descriptor.template get<FileDescriptor>();
		mesh = _loadMesh(meshDescriptor.name, _makeAbsolutePath(meshDescriptor.path));
	}
	return mesh;
}

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

std::vector<float> MeshLoader::_parseVertices(const std::string& strVertices) {
	return parseString<float>(strVertices, [] (const std::string& str) { return std::stof(str); });

}

std::vector<unsigned int> MeshLoader::_parseIndices(const std::string& strIndices) {
	return parseString<unsigned int>(strIndices, [] (const std::string& str) { return std::stoul(str); });
}

std::unique_ptr<Mesh> MeshLoader::_loadMesh(const std::string& name, const std::string& strVertices, const std::string& strIndices) {
	// TODO(Gerark) Using dynamic draw as usage but it should be best to receive this as a parameter
	auto meshVertices = std::make_unique<VertexBuffer>(_parseVertices(strVertices), GL_DYNAMIC_DRAW);
	meshVertices->bind();
	auto vertexAttributes = _createDefaultVertexAttributes();
	meshVertices->unbind();

	return std::make_unique<Mesh>(
		name,
		std::move(meshVertices),
		std::make_unique<IndexBuffer>(_parseIndices(strIndices), GL_DYNAMIC_DRAW),
		std::move(vertexAttributes)
	);
}

std::unique_ptr<Mesh> MeshLoader::_loadMesh(const std::string& name, const std::string& path) {
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

	// TODO(Gerark) Using dynamic draw as usage but it should be best to receive this as a parameter
	auto meshVertices = std::make_unique<VertexBuffer>(vertices, GL_DYNAMIC_DRAW);
	meshVertices->bind();
	auto vertexAttributes = _createDefaultVertexAttributes();
	meshVertices->unbind();

	return std::make_unique<Mesh>(
		name,
		std::move(meshVertices),
		std::make_unique<IndexBuffer>(indices, GL_DYNAMIC_DRAW),
		std::move(vertexAttributes)
	);
}

std::unique_ptr<VertexAttributes> MeshLoader::_createDefaultVertexAttributes() {
	auto builder = VertexAttributes::Builder();
	builder.attributef(3); // position
	builder.attributef(2); // uv
	auto attributes = builder.create();
	return attributes;
}

}
