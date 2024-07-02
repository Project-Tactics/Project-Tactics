#pragma once

#include "InputTypes.h"

namespace tactics::input {

void initContext();
Context& getContext();

InputActionId createAction(ActionType type, HashId name);
InputState inputState(InputActionId id);

void mapAction(InputActionId actionId, DeviceType deviceType, KeyAction keyAction, ActionTrigger trigger);

template<DeviceType DeviceType, typename TDeviceAction>
void mapAction(InputActionId actionId, TDeviceAction deviceAction, ActionTrigger trigger) {
	mapAction(actionId, DeviceType, deviceAction, trigger);
}

void _processKeyActions(KeyAction keyAction, bool pressed);

class InputSystem {
public:
};

} // namespace tactics::input
