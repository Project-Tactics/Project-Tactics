#include "MeshLoader.h"

#include <Libs/FileSystem/FileSystem.h>
#include <Libs/Utility/Exception.h>

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <regex>
#include <functional>

namespace tactics::resource {

struct MeshInlineDescriptor {
	std::string vertices;
	std::string indices;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(MeshInlineDescriptor, vertices, indices);
};

std::shared_ptr<Mesh> MeshLoader::load(const nlohmann::json& descriptor) {
	std::shared_ptr<Mesh> mesh;
	if (!descriptor.contains("path")) {
		auto meshDescriptor = descriptor.template get<MeshInlineDescriptor>();
		mesh = _loadMesh(meshDescriptor.vertices, meshDescriptor.indices);
	} else {
		auto meshDescriptor = descriptor.template get<FileDescriptor>();
		mesh = _loadMesh(_getFileSystem().getPathHelper().makeAbsolutePath(meshDescriptor.path));
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

std::shared_ptr<Mesh> MeshLoader::_loadMesh(const std::string& strVertices, const std::string& strIndices) {
	// TODO(Gerark) Using dynamic draw as usage but it should be best to receive this as a parameter
	auto meshVertices = std::make_unique<VertexBuffer>(_parseVertices(strVertices), rp::DynamicDraw::value);
	meshVertices->bind();
	auto vertexAttributes = _createDefaultVertexAttributes();
	meshVertices->unbind();

	auto mesh = std::make_shared<Mesh>(""_id);
	mesh->subMeshes.emplace_back(
		0,
		std::move(meshVertices),
		std::make_unique<IndexBuffer>(_parseIndices(strIndices), rp::DynamicDraw::value),
		std::move(vertexAttributes)
	);
	return mesh;
}

std::shared_ptr<Mesh> MeshLoader::_loadMesh(const std::string& path) {
	Assimp::Importer importer;
	// TODO(Gerark) We should have more control on how we load the meshes through filesystem and not relying on assimp to do the dirty job
	// assimp gives us the option to define our way of accessing files and filesystem ops.
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		throw TACTICS_EXCEPTION("Failed to load mesh: {}", importer.GetErrorString());
	}

	auto meshResource = std::make_shared<Mesh>();

	// TODO(Gerark) This is a very simple loader, it only loads the first UV channel and the vertices and indices of each mesh
	// It should be improved to load more data from the mesh like normals, tangents, bitangents, etc.
	for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		const aiMesh* mesh = scene->mMeshes[meshIndex];
		for (unsigned int vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {

			auto vertex = scene->mRootNode->mTransformation * mesh->mVertices[vertexIndex];

			vertices.push_back(vertex.x);
			vertices.push_back(vertex.y);
			vertices.push_back(vertex.z);

			unsigned int uvCount = mesh->GetNumUVChannels();
			for (unsigned int uvIndex = 0; uvIndex < uvCount; ++uvIndex) {
				if (mesh->HasTextureCoords(uvIndex)) {
					aiVector3D& uv = mesh->mTextureCoords[uvIndex][vertexIndex];
					vertices.push_back(uv.x);
					vertices.push_back(uv.y);
					break;
				}
			}
		}
		for (unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			const aiFace& face = mesh->mFaces[faceIndex];
			for (unsigned int index = 0; index < face.mNumIndices; ++index) {
				indices.push_back(face.mIndices[index]);
			}
		}

		// TODO(Gerark) Using dynamic draw is just temporary, we should have a way to define this through the descriptor
		auto meshVertices = std::make_unique<VertexBuffer>(vertices, rp::DynamicDraw::value);
		meshVertices->bind();
		auto vertexAttributes = _createDefaultVertexAttributes();
		meshVertices->unbind();

		meshResource->subMeshes.emplace_back(
			meshIndex,
			std::move(meshVertices),
			std::make_unique<IndexBuffer>(indices, rp::DynamicDraw::value),
			std::move(vertexAttributes)
		);
	}
	return meshResource;
}

std::unique_ptr<VertexAttributes> MeshLoader::_createDefaultVertexAttributes() {
	auto builder = VertexAttributes::Builder();
	builder.attributef(3); // position
	builder.attributef(2); // uv
	auto attributes = builder.create();
	return attributes;
}

}
