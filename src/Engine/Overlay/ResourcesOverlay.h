#pragma once

#include <Libs/Overlay/Overlay.h>

#include <Libs/Resource/Resource.h>

namespace tactics {
namespace resource {
class Pack;
class PackGroup;
class ResourceInfo;
class ResourceSystem;
}

class ResourcesOverlay: public Overlay {
public:
	ResourcesOverlay(resource::ResourceSystem& resourceSystem);
	void update() override;
	OverlayConfig getConfig() override;

private:
	void _drawByTypeTabItem();
	void _drawByPackTabItem();
	bool _drawResourcePackCollapsingHeader(const resource::Pack& pack, float colorMultiplier);
	bool _drawResourceTypeCollapsingHeader(const std::string& id, resource::ResourceType resourceType, unsigned int count, float colorMultiplier);
	bool _drawResourceCollapsingHeader(bool isLoaded, const std::string& name, resource::ResourceType resourceType, const resource::BaseResource* resource, float colorMultiplier);
	void _drawHeaderTypeLegend(const resource::Pack& pack);

	struct uiPackGroup {
		resource::ResourceType type;
		std::vector<const resource::ResourceInfo*> resources;
	};

	struct uiPack {
		const resource::Pack* pack;
		std::vector<uiPackGroup> groups;
	};

	std::tuple<unsigned int, unsigned int> _fillInfo();
	void _drawHeaderWithNumber(const std::string& text, size_t number);
	void _drawPacks(const std::vector<uiPack>& packs, float colorMultiplier);
	void _drawGroups(const std::string& id, const std::vector<uiPackGroup>& groups, float colorMultiplier);
	uiPackGroup& _getOrCreateGroup(std::vector<uiPackGroup>& groups, resource::ResourceType type);

	std::vector<uiPack> _loadedPacks;
	std::vector<uiPack> _unloadedPacks;
	std::vector<uiPackGroup> _loadedGroups;
	std::vector<uiPackGroup> _unloadedGroups;

	resource::ResourceSystem& _resourceSystem;
};

}
