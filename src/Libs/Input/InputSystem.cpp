#include "InputSystem.h"

namespace tactics::input {

Context* context = nullptr;

void initContext() { context = new Context{}; }

Context& getContext() { return *context; }

InputActionId createAction(ActionType type, HashId name) {
	auto id = static_cast<unsigned int>(context->actions.size());
	context->actions.emplace_back(id, name, type);
	return id;
}

InputState inputState(InputActionId actionId) { return context->actions[actionId].state; }

void mapAction(InputActionId actionId, DeviceType deviceType, KeyAction keyAction, ActionTrigger trigger) {
	Mapping mapping;
	mapping.actionId = actionId;
	DeviceInputMapping deviceInputMapping;
	deviceInputMapping.deviceType = deviceType;
	deviceInputMapping.deviceAction.key = keyAction;
	deviceInputMapping.triggers.push_back(trigger);

	context->mappings.push_back(mapping);
}

void _processKeyActions(KeyAction keyAction, bool pressed) {
	for (auto& mapping : context->mappings) {
		for (auto& deviceMapping : mapping.mapping) {
			if (deviceMapping.deviceType == DeviceType::Keyboard && deviceMapping.deviceAction.key == keyAction) {
				for (auto trigger : deviceMapping.triggers) {
					auto& action = context->actions[mapping.actionId];
					switch (trigger) {
					case ActionTrigger::Press:
						action.state = pressed ? InputState::Triggered : InputState::Ended;
						break;
					case ActionTrigger::Hold: action.state = pressed ? InputState::Started : InputState::Ended; break;
					case ActionTrigger::Release:
						action.state = pressed ? InputState::Ended : InputState::Triggered;
						break;
					}
				}
			}
		}
	}
}

} // namespace tactics::input
