#pragma once

#include <Libs/Resource/Resource.h>

#include <imgui/imgui.h>

namespace tactics {
namespace resource {
class BaseResource;
class IniFile;
class Mesh;
class Material;
class ResourceInfo;
class Shader;
class Texture;
}

class ResourceOverlayHelper {
public:
	static void drawResource(const resource::ResourceInfo& resourceInfo, resource::ResourceType resourceType);
	static void drawResource(const resource::IniFile& ini);
	static void drawResource(const resource::Material& material);
	static void drawResource(const resource::Mesh& mesh);
	static void drawResource(const resource::Shader& shader);
	static void drawResource(const resource::Texture& texture);
	static ImVec4 toColor(resource::ResourceType type, float multiplier = 1.f, float alphaMultiplier = 1.f);

private:
	static bool _beginTabItem(const char* name, resource::ResourceType resourceType);

	template<typename TResource>
	static void drawTResource(const resource::BaseResource& resource) {
		auto& tResource = static_cast<const TResource&>(resource);
		drawResource(tResource);
	}

};

}
