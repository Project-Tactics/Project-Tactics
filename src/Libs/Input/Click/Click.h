#pragma once

#include "ClickInputTypes.h"

/*
 * Click is a simple input library which could be extracted by the engine and be standalone
 * with the right amount of work and refactoring.
 */
namespace click {

void initializeContext();
Context& getContext();

InputActionId createAction(ActionType type, NameId name);
InputState inputState(InputActionId id);

void mapAction(InputActionId actionId, DeviceType deviceType, DeviceAction keyAction, ActionTrigger trigger);

template<DeviceType DeviceType>
void mapAction(InputActionId actionId, DeviceAction deviceAction, ActionTrigger trigger) {
	mapAction(actionId, DeviceType, deviceAction, trigger);
}

void _processKeyActions(KeyboardAction keyAction, bool pressed);

} // namespace click
