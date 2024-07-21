#pragma once

#include <Libs/Input/InputSystem.h>
#include <Libs/Overlay/Overlay.h>

#include <string>
#include <unordered_map>
#include <vector>

namespace tactics {
namespace resource {
class ResourceProvider;
}

class InputOverlay : public Overlay {
public:
	InputOverlay(InputSystem& inputSystem, resource::ResourceProvider& resourceProvider);
	void update() override;
	OverlayConfig getConfig() override;

private:
	void _collectInfo();
	ImVec4 _interpolateColors(const std::vector<ImVec4>& colors, float value) const;
	ImVec4 _getColorFromMagnitude(float magnitude) const;

	struct InputValue {
		click::ActionValue value;
		click::InputCode inputCode{};
		float magnitude{};
	};

	struct InputValues {
		std::vector<InputValue> values;
		float maxMagnitude{};
	};

	void _drawDevicesInputValues(const std::unordered_map<click::DeviceType, InputValues>& devices);
	void _drawHeldDevices(click::PlayerId playerIndex);
	void _drawActionMaps(click::PlayerId playerIndex);
	void _drawInputInspector();
	void
	_drawInputCodeValue(click::InputCode inputCode, const click::ActionValue& value, float magnitude, float yOffset);

	InputSystem& _inputSystem;
	resource::ResourceProvider& _resourceProvider;
	std::vector<std::unordered_map<click::DeviceType, InputValues>> _actionValues;

	bool _inputInspectorOpen = false;
	std::vector<click::InputCode> _inspectedInputCodes;
	std::vector<std::string> _inputCodesChoices;
	int _currentInspectedInputCode{};
	int _currentPlayerInspected{};
};

} // namespace tactics
