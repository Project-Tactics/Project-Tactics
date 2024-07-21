#include "InputOverlay.h"

#include "CustomOverlayColors.h"

#include <Libs/Input/InputSerialization.h>
#include <Libs/Resource/Input/InputAction.h>
#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Utility/String/String.h>

#include <imgui/imgui.h>

using namespace std::literals::string_literals;

namespace tactics {

InputOverlay::InputOverlay(InputSystem& inputSystem, resource::ResourceProvider& resourceProvider)
	: _inputSystem(inputSystem)
	, _resourceProvider(resourceProvider) {
	for (auto i = 0u; i < static_cast<unsigned int>(click::InputCode::Count); ++i) {
		_inputCodesChoices.push_back(toString(static_cast<click::InputCode>(i)));
	}
}

OverlayConfig InputOverlay::getConfig() {
	OverlayConfig config;
	config.position = {5, 30};
	config.size = {300, 0};
	config.isMenuBarButton = true;
	return config;
}

void InputOverlay::update() {
	if (ImGui::Button("Toggle Inspect")) {
		_inputInspectorOpen = !_inputInspectorOpen;
	}

	if (_inputInspectorOpen) {
		_drawInputInspector();
	}

	ImGui::BeginTabBar("Players");

	_collectInfo();

	for (click::PlayerId playerIndex = 0u; playerIndex < _actionValues.size(); ++playerIndex) {
		if (ImGui::BeginTabItem(("Player"s + toString(playerIndex)).c_str())) {
			_drawDevicesInputValues(_actionValues[playerIndex]);
			_drawHeldDevices(playerIndex);
			_drawActionMaps(playerIndex);

			ImGui::EndTabItem();
		}
	}

	ImGui::EndTabBar();
}

void InputOverlay::_drawActionMaps(click::PlayerId playerIndex) {
	if (ImGui::CollapsingHeader("Input Action")) {
		ImGui::Indent();

		auto& manager = _resourceProvider.getManager(resource::ResourceType::InputAction);
		manager.forEachTResource<resource::InputAction>([&](resource::InputAction& inputAction) {
			auto name = fmt::format("{}##{}{}", inputAction.name, inputAction.id, playerIndex);
			auto& actionState = click::actionState(inputAction.actionId, playerIndex);

			int styleToPop = 0;
			if (actionState.state == click::InputState::Triggered) {
				auto color = CustomOverlayColors::getColors().InputChangeColorVeryLow;
				ImGui::PushStyleColor(ImGuiCol_Header, ImGui::ColorConvertFloat4ToU32(color));
				++styleToPop;
			}

			auto collapsed = ImGui::CollapsingHeader(name.c_str());
			ImGui::PopStyleColor(styleToPop);

			if (collapsed) {
				ImGui::Indent();
				auto state = fmt::format("State: {}", actionState.state);
				auto value = fmt::format("Value: {}", actionState.value);
				ImGui::Text(state.c_str());
				ImGui::Text(value.c_str());
				ImGui::Unindent();
			}
		});

		ImGui::Unindent();
	}
}

void InputOverlay::_drawHeldDevices(click::PlayerId playerIndex) {
	auto& player = _inputSystem.getPlayer(playerIndex);
	if (player.heldDevices.empty()) {
		return;
	}

	if (ImGui::CollapsingHeader("Held Devices")) {
		ImGui::Indent();
		for (auto& deviceId : player.heldDevices) {
			auto& deviceData = _inputSystem.getDevice(deviceId);
			ImGui::Text(fmt::format("{}", deviceData.type).c_str());
		}
		ImGui::Unindent();
	}
}

void InputOverlay::_drawDevicesInputValues(const std::unordered_map<click::DeviceType, InputValues>& devices) {
	if (ImGui::CollapsingHeader("Devices")) {
		ImGui::Indent();
		for (auto& [deviceType, values] : devices) {
			int styleToPop = 0;
			if (values.maxMagnitude > 0) {
				auto color = _getColorFromMagnitude(values.maxMagnitude);
				ImGui::PushStyleColor(ImGuiCol_Header, ImGui::ColorConvertFloat4ToU32(color));
				++styleToPop;
			}

			auto collapsed = ImGui::CollapsingHeader(fmt::format("{}", deviceType).c_str());
			ImGui::PopStyleColor(styleToPop);

			if (collapsed) {
				for (auto& [value, inputCode, magnitude] : values.values) {
					_drawInputCodeValue(inputCode, value, magnitude, 0.f);
				}
			}
		}
		ImGui::Unindent();
	}
}

void InputOverlay::_drawInputCodeValue(click::InputCode inputCode,
									   const click::ActionValue& value,
									   float magnitude,
									   float yOffset) {
	auto inputCodeString = fmt::format("{}: {}", inputCode, value);
	if (magnitude > 0) {
		auto color = _getColorFromMagnitude(magnitude);
		auto size = ImGui::CalcTextSize(inputCodeString.c_str());
		ImVec2 cursorPos = ImGui::GetCursorScreenPos();
		cursorPos.x -= 5.f;
		cursorPos.y -= 2.f + yOffset;
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		drawList->AddRectFilled(cursorPos,
								ImVec2(cursorPos.x + size.x + 10.f, cursorPos.y + size.y + 4),
								ImGui::ColorConvertFloat4ToU32(color));
	}
	ImGui::Text(inputCodeString.c_str());
}

void InputOverlay::_collectInfo() {
	_actionValues.clear();
	auto playerCount = _inputSystem.getNumPlayers();
	_actionValues.resize(playerCount);
	for (click::PlayerId playerIndex = 0u; playerIndex < playerCount; ++playerIndex) {
		auto& player = _inputSystem.getPlayer(playerIndex);
		for (auto inputCodeIndex = 0; auto& value : player.inputValues) {
			auto inputCode = static_cast<click::InputCode>(inputCodeIndex);
			auto deviceType = click::getDeviceTypeFromInputCode(inputCode);
			if (_inputSystem.hasDeviceAssigned(playerIndex, deviceType)) {
				auto& device = _actionValues[playerIndex][deviceType];
				auto magnitude = click::_magnitudeSquared(value);
				device.values.push_back({value, inputCode, magnitude});
				device.maxMagnitude = std::max(magnitude, device.maxMagnitude);
			}
			++inputCodeIndex;
		}
	}
}

ImVec4 InputOverlay::_getColorFromMagnitude(float magnitude) const {
	return _interpolateColors({CustomOverlayColors::getColors().InputChangeColorVeryLow,
							   CustomOverlayColors::getColors().InputChangeColorLow,
							   CustomOverlayColors::getColors().InputChangeColorMid,
							   CustomOverlayColors::getColors().InputChangeColorHigh},
							  magnitude);
}

ImVec4 InputOverlay::_interpolateColors(const std::vector<ImVec4>& colors, float value) const {
	if (value <= 0) {
		return colors[0];
	}

	if (value >= 1) {
		return colors.back();
	}

	auto index = static_cast<size_t>(value * (colors.size() - 1));
	auto remainder = value * (colors.size() - 1) - index;
	auto& color1 = colors[index];
	auto& color2 = colors[index + 1];
	return ImVec4{
		color1.x + (color2.x - color1.x) * remainder,
		color1.y + (color2.y - color1.y) * remainder,
		color1.z + (color2.z - color1.z) * remainder,
		color1.w + (color2.w - color1.w) * remainder,
	};
}

void InputOverlay::_drawInputInspector() {
	if (ImGui::Begin("Input Inspector", &_inputInspectorOpen)) {
		ImGui::Text("Input Inspector");
		std::vector<const char*> items;
		items.reserve(_inputCodesChoices.size());
		for (const auto& item : _inputCodesChoices) {
			items.push_back(item.c_str());
		}

		std::array players{
			"Player 0",
			"Player 1",
			"Player 2",
			"Player 3",
			"Player 4",
			"Player 5",
			"Player 6",
			"Player 7",
			"Player 8",
		};
		ImGui::Combo("##PlayerCombo", &_currentPlayerInspected, players.data(), _inputSystem.getNumPlayers(), 4);

		auto addButtonClicked = ImGui::Button("Add");
		ImGui::SameLine();
		ImGui::Combo("##InputCodeCombo", &_currentInspectedInputCode, items.data(), static_cast<int>(items.size()), 5);

		if (addButtonClicked) {
			_inspectedInputCodes.push_back(static_cast<click::InputCode>(_currentInspectedInputCode));
		}

		ImGui::Separator();

		for (auto i = 0u; i < _inspectedInputCodes.size(); ++i) {
			auto& inputCode = _inspectedInputCodes[i];

			auto& value =
				_inputSystem.getInputCodeValue(inputCode, static_cast<click::PlayerId>(_currentPlayerInspected));
			auto magnitude = click::_magnitudeSquared(value);

			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 0.0f, 0.0f, 0.25f));
			auto buttonLabel = fmt::format("X##{}", i);
			if (ImGui::Button(buttonLabel.c_str())) {
				_inspectedInputCodes.erase(_inspectedInputCodes.begin() + i);
			}
			ImGui::PopStyleColor(1);

			ImGui::SameLine();
			ImGui::AlignTextToFramePadding();
			_drawInputCodeValue(inputCode, value, magnitude, -5.f);
		}
	}
	ImGui::End();
}

} // namespace tactics
