#include "ResourcesOverlay.h"

#include "ResourceOverlayHelper.h"

#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Utility/String.h>

#include <fmt/format.h>

namespace tactics {

ResourcesOverlay::ResourcesOverlay(resource::ResourceSystem& resourceSystem) : _resourceSystem(resourceSystem) {}

OverlayConfig ResourcesOverlay::getConfig() {
	OverlayConfig config;
	config.position = {310, 30};
	config.size = {300, 0};
	config.isMenuBarButton = true;
	return config;
}

void ResourcesOverlay::update() {
	ImGui::BeginTabBar("ResourcesTabBar");
	if (ImGui::BeginTabItem("By Type")) {
		_drawByTypeTabItem();
		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("By Pack")) {
		_drawByPackTabItem();
		ImGui::EndTabItem();
	}
	ImGui::EndTabBar();
}

void ResourcesOverlay::_drawByTypeTabItem() {
	using namespace resource;

	auto [loadedCount, unloadedCount] = _fillInfo();

	if (!_loadedGroups.empty()) {
		_drawHeaderWithNumber("Loaded", loadedCount);
		_drawGroups("loaded"_id, _loadedGroups, 1.0f);
	}

	if (!_unloadedGroups.empty()) {
		_drawHeaderWithNumber("Unloaded", unloadedCount);
		_drawGroups("unloaded"_id, _unloadedGroups, 0.5f);
	}
}

void ResourcesOverlay::_drawByPackTabItem() {
	using namespace resource;

	auto [loadedCount, unloadedCount] = _fillInfo();

	if (!_loadedPacks.empty()) {
		_drawHeaderWithNumber("Loaded", loadedCount);
		_drawPacks(_loadedPacks, 1.f);
	}

	if (!_unloadedPacks.empty()) {
		_drawHeaderWithNumber("Unloaded", unloadedCount);
		_drawPacks(_unloadedPacks, 0.5f);
	}
}

void ResourcesOverlay::_drawHeaderWithNumber(const std::string& text, size_t number) {
	auto header = fmt::format("{} [{}]", text, number);
	ImGui::Separator();
	ImGui::Text(header.c_str());
	ImGui::Separator();
}

void ResourcesOverlay::_drawPacks(const std::vector<uiPack>& packs, float colorMultiplier) {
	for (auto& pack : packs) {
		if (_drawResourcePackCollapsingHeader(*pack.pack, colorMultiplier)) {
			ImGui::Indent();
			_drawGroups(pack.pack->getName(), pack.groups, colorMultiplier);
			ImGui::Unindent();
		}
	}
}

void ResourcesOverlay::_drawGroups(const HashId& id, const std::vector<uiPackGroup>& groups, float colorMultiplier) {
	for (auto& group : groups) {
		if (_drawResourceTypeCollapsingHeader(id,
											  group.type,
											  static_cast<unsigned int>(group.resources.size()),
											  colorMultiplier)) {
			ImGui::Indent();
			for (auto& resource : group.resources) {
				if (_drawResourceCollapsingHeader(resource->isLoaded(),
												  resource->getName(),
												  group.type,
												  resource->getResource().get(),
												  colorMultiplier)) {
					ImGui::Indent();
					ResourceOverlayHelper::drawResource(*resource, group.type);
					ImGui::Unindent();
				}
			}
			ImGui::Unindent();
		}
	}
}

bool ResourcesOverlay::_drawResourceCollapsingHeader(bool isLoaded,
													 const HashId& name,
													 resource::ResourceType resourceType,
													 const resource::BaseResource* resource,
													 float colorMultiplier) {
	using namespace resource;
	auto id = isLoaded ? toString(resource->id) : "X";
	auto resourceHeader = fmt::format("[ID:{}] {}", id, name);

	ImGui::PushStyleColor(ImGuiCol_Header, ResourceOverlayHelper::toColor(resourceType, 0.4f * colorMultiplier));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ResourceOverlayHelper::toColor(resourceType, 0.5f * colorMultiplier));
	bool collapsed = ImGui::CollapsingHeader(resourceHeader.c_str());
	ImGui::PopStyleColor(2);
	return collapsed;
}

bool ResourcesOverlay::_drawResourceTypeCollapsingHeader(const HashId& id,
														 resource::ResourceType resourceType,
														 unsigned int count,
														 float colorMultiplier) {
	auto resourceTypeString = toString(resourceType);
	std::transform(resourceTypeString.begin(), resourceTypeString.end(), resourceTypeString.begin(), tactics::toupper);
	auto groupHeader = fmt::format("{} [{}]##{}", resourceTypeString, count, id);

	ImGui::PushStyleColor(ImGuiCol_Header, ResourceOverlayHelper::toColor(resourceType, 0.5f * colorMultiplier));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ResourceOverlayHelper::toColor(resourceType, 0.6f * colorMultiplier));
	bool collapsed = ImGui::CollapsingHeader(groupHeader.c_str());
	ImGui::PopStyleColor(2);

	if (collapsed && count == 0) {
		ImGui::Indent();
		ImGui::Text("No resources");
		ImGui::Unindent();
		return false;
	}
	return collapsed;
}

bool ResourcesOverlay::_drawResourcePackCollapsingHeader(const resource::Pack& pack, float colorMultiplier) {
	auto loadedString = pack.isLoaded() ? "LOADED" : "";
	auto packHeader = fmt::format("{} [{}] {}", pack.getName(), pack.getResourceCount(), loadedString);
	auto color = ImVec4{0.2f, 0.18f, 0.24f, 1.00f};
	color.x *= colorMultiplier;
	color.y *= colorMultiplier;
	color.z *= colorMultiplier;
	ImGui::PushStyleColor(ImGuiCol_Header, color);
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, {0.26f, 0.26f, 0.28f, 1.00f});
	bool collapsed = ImGui::CollapsingHeader(packHeader.c_str());
	ImGui::PopStyleColor(2);

	_drawHeaderTypeLegend(pack);

	if (collapsed && pack.isManuallyCreated() && !pack.isLoaded() && pack.getResourceCount() == 0) {
		ImGui::Indent();
		ImGui::TextWrapped(
			"This is a manually created pack. When these types of packs are unloaded all the resources are removed and "
			"no metadata is kept.");
		ImGui::Unindent();
		return false;
	}

	return collapsed;
}

void ResourcesOverlay::_drawHeaderTypeLegend(const resource::Pack& pack) {
	std::vector<resource::ResourceType> resourceTypes;
	pack.forEachGroup([&resourceTypes](const resource::PackGroup& group) { resourceTypes.push_back(group.getType()); });

	for (auto&& resourceType : resourceTypes) {
		ImGui::SameLine();
		ImVec2 squareSize(5, 5);
		ImVec2 cursorPos = ImGui::GetCursorScreenPos();
		cursorPos.y += 10;
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddRectFilled(cursorPos,
								ImVec2(cursorPos.x + squareSize.x, cursorPos.y + squareSize.y),
								ImGui::ColorConvertFloat4ToU32(ResourceOverlayHelper::toColor(resourceType, 0.6f)));
		auto dummySize = ImVec2(squareSize.x - 8, squareSize.y);
		ImGui::Dummy(dummySize);
	}
}

std::tuple<unsigned int, unsigned int> ResourcesOverlay::_fillInfo() {
	using namespace resource;

	_loadedPacks.clear();
	_unloadedPacks.clear();
	_loadedGroups.clear();
	_unloadedGroups.clear();

	unsigned int loadedCount = 0;
	unsigned int unloadedCount = 0;
	_resourceSystem.forEachPack([this, &loadedCount, &unloadedCount](const Pack& pack) {
		unsigned int* count;
		std::vector<uiPack>* packs;
		std::vector<uiPackGroup>* groups;
		if (pack.isLoaded()) {
			packs = &_loadedPacks;
			groups = &_loadedGroups;
			count = &loadedCount;
		} else {
			packs = &_unloadedPacks;
			groups = &_unloadedGroups;
			count = &unloadedCount;
		}
		packs->push_back({&pack});
		uiPack& uiPack = packs->back();

		pack.forEachGroup([this, &uiPack, &count, &groups](const PackGroup& group) {
			uiPack.groups.push_back({group.getType()});

			auto& uiGroupForTypes = _getOrCreateGroup(*groups, group.getType());
			auto& uiGroupForPacks = uiPack.groups.back();

			group.forEachResource([&uiGroupForPacks, &uiGroupForTypes, &count](const ResourceInfo& resourceInfo) {
				uiGroupForPacks.resources.push_back(&resourceInfo);
				uiGroupForTypes.resources.push_back(&resourceInfo);
				++(*count);
			});
		});
	});
	return {loadedCount, unloadedCount};
}

ResourcesOverlay::uiPackGroup& ResourcesOverlay::_getOrCreateGroup(std::vector<uiPackGroup>& groups,
																   resource::ResourceType type) {
	auto itr = std::ranges::find_if(groups, [type](const uiPackGroup& uiGroup) { return uiGroup.type == type; });
	if (itr == groups.end()) {
		groups.push_back({type});
		return groups.back();
	}
	return *itr;
}

} // namespace tactics
