#include "../Overlay/PrototypeOverlay.h"

#include "../Component/CharacterComponent.h"

#include <Engine/Overlay/CustomOverlayColors.h>

#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Rendering/RenderSystem.h>
#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Utility/ImGuiUtilities.h>

namespace tactics {

PrototypeOverlay::PrototypeOverlay(RenderSystem& renderSystem,
								   EntityComponentSystem& ecs,
								   resource::ResourceSystem& resourceSystem)
	: _renderSystem(renderSystem)
	, _resourceSystem(resourceSystem)
	, _ecs(ecs) {}

OverlayConfig PrototypeOverlay::getConfig() {
	OverlayConfig config;
	config.position = {5, 30};
	config.size = {300, 0};
	config.isMenuBarButton = true;
	return config;
}

void PrototypeOverlay::update() {
	CenteredTextColored(CustomOverlayColors::Colors::TitleTextColor, "Characters");

	using namespace component;
	auto view = _ecs.sceneRegistry().view<CharName, CharAvatar, CharStats, ChargeTime>();

	// Count entities to determine column count
	std::vector<std::tuple<const CharName*, const CharAvatar*, const CharStats*, const ChargeTime*, bool>> characters;
	for (auto [entity, charName, charAvatar, charStats, chargeTime] : view.each()) {
		auto& tuple = characters.emplace_back(&charName, &charAvatar, &charStats, &chargeTime, false);
		std::get<4>(tuple) = _ecs.sceneRegistry().any_of<CharBattleReady>(entity) ? true : false;
	}
	int columnCount = static_cast<int>(characters.size());
	if (columnCount > 0 &&
		ImGui::BeginTable("CharacterTable", columnCount, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
		// Header row: Character names
		ImGui::TableNextRow();
		for (int col = 0; col < columnCount; ++col) {
			ImGui::TableSetColumnIndex(col);
			ImGui::Text("%s", std::get<0>(characters[col])->name.c_str());
		}
		// Avatar row
		ImGui::TableNextRow();
		for (int col = 0; col < columnCount; ++col) {
			ImGui::TableSetColumnIndex(col);
			auto charAvatar = std::get<1>(characters[col]);
			auto texture = _resourceSystem.getResource<resource::Texture>(charAvatar->textureId);
			auto width = static_cast<float>(texture->info.width);
			auto height = static_cast<float>(texture->info.height);
			auto ratio = height / width;
			ImGui::Image((void*)(intptr_t)texture->rendererId, ImVec2(64, 64 * ratio), ImVec2(0, 1), ImVec2(1, 0));
		}
		// Charge Time row
		ImGui::TableNextRow();
		for (int col = 0; col < columnCount; ++col) {
			ImGui::TableSetColumnIndex(col);
			if (std::get<4>(characters[col])) {
				ImGui::TextColored({0, 1, 0, 1}, "READY");
				ImGui::Button("DO ACTION");
			} else {
				ImGui::TextColored({1, 1, 1, 1}, "CHARGING");
				auto chargeTime = std::get<3>(characters[col]);
				ImGui::Text("CT: %.2f", chargeTime->chargeTime);
				ImGui::ProgressBar(chargeTime->chargeTime / 100.0f, ImVec2(-1, 0), "");
			}
		}
		// HP row
		ImGui::TableNextRow();
		for (int col = 0; col < columnCount; ++col) {
			ImGui::TableSetColumnIndex(col);
			auto charStats = std::get<2>(characters[col]);
			ImGui::Text("HP: %d/%d", charStats->hp, charStats->maxHp);
		}
		// MP row
		ImGui::TableNextRow();
		for (int col = 0; col < columnCount; ++col) {
			ImGui::TableSetColumnIndex(col);
			auto charStats = std::get<2>(characters[col]);
			ImGui::Text("MP: %d/%d", charStats->mp, charStats->maxMp);
		}
		// Level row
		ImGui::TableNextRow();
		for (int col = 0; col < columnCount; ++col) {
			ImGui::TableSetColumnIndex(col);
			auto charStats = std::get<2>(characters[col]);
			ImGui::Text("Level: %d", charStats->level);
		}
		// XP row
		ImGui::TableNextRow();
		for (int col = 0; col < columnCount; ++col) {
			ImGui::TableSetColumnIndex(col);
			auto charStats = std::get<2>(characters[col]);
			ImGui::Text("XP: %d", charStats->xp);
		}
		// Move row
		ImGui::TableNextRow();
		for (int col = 0; col < columnCount; ++col) {
			ImGui::TableSetColumnIndex(col);
			auto charStats = std::get<2>(characters[col]);
			ImGui::Text("Move: %d", charStats->move);
		}
		// Jump row
		ImGui::TableNextRow();
		for (int col = 0; col < columnCount; ++col) {
			ImGui::TableSetColumnIndex(col);
			auto charStats = std::get<2>(characters[col]);
			ImGui::Text("Jump: %d", charStats->jump);
		}
		// Speed row
		ImGui::TableNextRow();
		for (int col = 0; col < columnCount; ++col) {
			ImGui::TableSetColumnIndex(col);
			auto chargeTime = std::get<3>(characters[col]);
			ImGui::Text("Speed: %.2f", chargeTime->speed);
		}
		ImGui::EndTable();
	}
}

} // namespace tactics
