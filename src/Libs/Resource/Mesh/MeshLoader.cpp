#include "MeshLoader.h"

#include <Libs/FileSystem/FileSystem.h>
#include <Libs/Utility/Exception.h>

#include <assimp/Importer.hpp>	// C++ importer interface
#include <assimp/postprocess.h> // Post processing flags
#include <assimp/scene.h>		// Output data structure
#include <functional>
#include <glm/glm.hpp>
#include <regex>

namespace tactics::resource {

struct MeshInlineDescriptor {
	std::string vertices;
	std::string indices;

	JSON_SERIALIZE(MeshInlineDescriptor, vertices, indices);
};

namespace {

constexpr unsigned int legacyInlineVertexComponents = 5;
constexpr unsigned int defaultVertexComponents = 8;

bool indicesFitVertexCount(const std::vector<unsigned int>& indices, unsigned int vertexCount) {
	for (auto index : indices) {
		if (index >= vertexCount) {
			return false;
		}
	}
	return true;
}

unsigned int vertexCountForLayout(const std::vector<float>& vertices, unsigned int componentsPerVertex) {
	if (vertices.empty() || vertices.size() % componentsPerVertex != 0) {
		return 0;
	}

	return static_cast<unsigned int>(vertices.size() / componentsPerVertex);
}

std::vector<float> expandLegacyInlineVertices(const std::vector<float>& vertices) {
	std::vector<float> expandedVertices;
	auto vertexCount = vertexCountForLayout(vertices, legacyInlineVertexComponents);
	expandedVertices.reserve(vertexCount * defaultVertexComponents);

	for (auto vertexIndex = 0u; vertexIndex < vertexCount; ++vertexIndex) {
		auto offset = vertexIndex * legacyInlineVertexComponents;
		expandedVertices.insert(expandedVertices.end(),
								vertices.begin() + offset,
								vertices.begin() + offset + legacyInlineVertexComponents);
		expandedVertices.push_back(0.0f);
		expandedVertices.push_back(0.0f);
		expandedVertices.push_back(1.0f);
	}

	return expandedVertices;
}

std::vector<float> normalizeInlineVertices(std::vector<float> vertices, const std::vector<unsigned int>& indices) {
	auto defaultVertexCount = vertexCountForLayout(vertices, defaultVertexComponents);
	auto legacyVertexCount = vertexCountForLayout(vertices, legacyInlineVertexComponents);

	if (defaultVertexCount > 0 && indicesFitVertexCount(indices, defaultVertexCount)) {
		return vertices;
	}

	if (legacyVertexCount > 0 && indicesFitVertexCount(indices, legacyVertexCount)) {
		return expandLegacyInlineVertices(vertices);
	}

	TACTICS_EXCEPTION(
		"Inline mesh vertex data must use either 5 floats per vertex (position + uv) or 8 floats per "
		"vertex (position + uv + normal), and all indices must fit the resolved vertex count. "
		"Got {} vertex floats and {} indices.",
		vertices.size(),
		indices.size());
	return {};
}

} // namespace

std::shared_ptr<Mesh> MeshLoader::load(const json& descriptor) {
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
	return parseString<float>(strVertices, [](const std::string& str) { return std::stof(str); });
}

std::vector<unsigned int> MeshLoader::_parseIndices(const std::string& strIndices) {
	return parseString<unsigned int>(strIndices, [](const std::string& str) { return std::stoul(str); });
}

std::shared_ptr<Mesh> MeshLoader::_loadMesh(const std::string& strVertices, const std::string& strIndices) {
	// TODO(Gerark) Using dynamic draw as usage but it should be best to receive this as a parameter
	auto indices = _parseIndices(strIndices);
	auto vertices = normalizeInlineVertices(_parseVertices(strVertices), indices);
	auto vb = VertexBuffer(vertices, rp::DynamicDraw::value);
	auto ib = IndexBuffer(indices, rp::DynamicDraw::value);
	auto vertexAttributes = _createDefaultVertexAttributes(vb, ib);

	auto mesh = std::make_shared<Mesh>(""_id);
	mesh->subMeshes.emplace_back(0, std::move(vb), std::move(ib), std::move(vertexAttributes));
	return mesh;
}

std::shared_ptr<Mesh> MeshLoader::_loadMesh(const std::string& path) {
	Assimp::Importer importer;
	// TODO(Gerark) We should have more control on how we load the meshes through filesystem and not relying on assimp
	// to do the dirty job assimp gives us the option to define our way of accessing files and filesystem ops.
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		TACTICS_EXCEPTION("Failed to load mesh: {}", importer.GetErrorString());
	}

	auto meshResource = std::make_shared<Mesh>();

	// TODO(Gerark) This is a very simple loader, it only loads the first UV channel and the vertices and indices of
	// each mesh It should be improved to load more data from the mesh like normals, tangents, bitangents, etc.
	for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		const aiMesh* mesh = scene->mMeshes[meshIndex];
		for (unsigned int vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex) {
			auto vertex = scene->mRootNode->mTransformation * mesh->mVertices[vertexIndex];

			vertices.push_back(vertex.x);
			vertices.push_back(vertex.y);
			vertices.push_back(vertex.z);

			// Sort of expecting only one set of uv coordinates so this would create artifacts if there are more
			unsigned int uvCount = mesh->GetNumUVChannels();
			auto hasUv = false;
			for (unsigned int uvIndex = 0; uvIndex < uvCount; ++uvIndex) {
				if (mesh->HasTextureCoords(uvIndex)) {
					aiVector3D& uv = mesh->mTextureCoords[uvIndex][vertexIndex];
					vertices.push_back(uv.x);
					vertices.push_back(uv.y);
					hasUv = true;
					break;
				}
			}
			if (!hasUv) {
				vertices.push_back(0.0f);
				vertices.push_back(0.0f);
			}

			if (mesh->HasNormals()) {
				aiVector3D& normal = mesh->mNormals[vertexIndex];
				vertices.push_back(normal.x);
				vertices.push_back(normal.y);
				vertices.push_back(normal.z);
			} else {
				// Kind of fallback since we expect normals anyway
				vertices.push_back(0.0f);
				vertices.push_back(0.0f);
				vertices.push_back(0.0f);
			}
		}
		for (unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex) {
			const aiFace& face = mesh->mFaces[faceIndex];
			for (unsigned int index = 0; index < face.mNumIndices; ++index) {
				indices.push_back(face.mIndices[index]);
			}
		}

		// TODO(Gerark) Using dynamic draw is just temporary, we should have a way to define this through the descriptor
		auto vb = VertexBuffer(vertices, rp::DynamicDraw::value);
		auto ib = IndexBuffer(indices, rp::DynamicDraw::value);
		auto vertexAttributes = _createDefaultVertexAttributes(vb, ib);

		meshResource->subMeshes.emplace_back(meshIndex, std::move(vb), std::move(ib), std::move(vertexAttributes));
	}
	return meshResource;
}

VertexAttributes MeshLoader::_createDefaultVertexAttributes(VertexBuffer& vb, IndexBuffer& ib) {
	auto builder = VertexAttributes::Builder();
	builder.attributef(3); // position
	builder.attributef(2); // uv
	builder.attributef(3); // normal
	return builder.create(vb, ib);
}

} // namespace tactics::resource
