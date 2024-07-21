#include "InputOverlay.h"

#include "CustomOverlayColors.h"

#include <Libs/Input/InputSerialization.h>
#include <Libs/Input/InputSystem.h>
#include <Libs/Utility/String/String.h>

#include <imgui/imgui.h>

using namespace std::literals::string_literals;

namespace tactics {

InputOverlay::InputOverlay(InputSystem& inputSystem) : _inputSystem(inputSystem) {}

void InputOverlay::update() {
	ImGui::BeginTabBar("Players");

	auto playerCount = _inputSystem.getNumPlayers();
	for (click::PlayerId playerIndex = 0u; playerIndex < playerCount; ++playerIndex) {
		if (ImGui::BeginTabItem(("Player"s + toString(playerIndex)).c_str())) {
			auto& player = _inputSystem.getPlayer(playerIndex);

			auto previousDeviceType = click::DeviceType::None;
			bool collapsed = false;
			for (auto inputCode = 0; auto& value : player.inputValues) {
				auto deviceType = click::getDeviceTypeFromInputCode(static_cast<click::InputCode>(inputCode));
				if (previousDeviceType != deviceType) {
					previousDeviceType = deviceType;
					collapsed = ImGui::CollapsingHeader(fmt::format("{}", deviceType).c_str());
				}

				if (collapsed) {
					ImGui::Text("InputCode: %.02f, %.02f, %.02f", value.vec3.x, value.vec3.y, value.vec3.z);
				}
				++inputCode;
			}

			ImGui::EndTabItem();
		}
	}

	ImGui::EndTabBar();
}

OverlayConfig InputOverlay::getConfig() {
	OverlayConfig config;
	config.position = {5, 30};
	config.size = {300, 0};
	config.isMenuBarButton = true;
	return config;
}

} // namespace tactics
