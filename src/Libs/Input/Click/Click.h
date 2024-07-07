#pragma once

#include "ClickEvents.h"
#include "ClickInputTypes.h"

/*
 * Click is a simple input library which could be extracted by the engine and be standalone
 * with the right amount of work and refactoring.
 */
namespace click {

void initializeContext();
Context& getContext();
void update(float deltaTime);

OwnerId owner();

/*
 * Device
 */

void installGamepad(void* customData);
void installKeyboard();
void installMouse();

void uninstallMouse(DeviceId id);
void uninstallKeyboard(DeviceId id);
void uninstallGamepad(DeviceId id);

DeviceId keyboard(unsigned int index);
DeviceId mouse(unsigned int index);
DeviceId gamepad(unsigned int index);
DeviceType deviceType(DeviceId id);
DeviceData& device(DeviceId id);

const DeviceIdList& gamepads();
const DeviceIdList& mice();
const DeviceIdList& keyboards();
const DeviceIdList& touches();

/*
 * Actions Creation
 */
InputActionId impulse();
InputActionId axis1D();
InputActionId axis2D();
InputActionId axis3D();

std::tuple<InputState, bool> impulseState(InputActionId id);
std::tuple<InputState, float> axis1DState(InputActionId id);
std::tuple<InputState, Axis2D> axis2DState(InputActionId id);
std::tuple<InputState, Axis3D> axis3DState(InputActionId id);

ActionType type(InputActionId id);

/*
 * Trigger Creation
 */
Trigger downTrigger(float actuationThreshold);
Trigger pressTrigger(float actuationThreshold);
Trigger releaseTrigger(float actuationThreshold);
Trigger holdTrigger(float actuationThreshold, float holdTime);

/*
 * Modifier Creation
 */
Modifier negateModifier();

/*
 * Mapping
 */
void map(InputActionId actionId,
		 DeviceId deviceId,
		 const DeviceGestureData& gestureData,
		 std::vector<Trigger> triggers,
		 std::vector<Modifier> modifiers);

/*
 * Event Processing
 */
void processKeyboardEvent(const KeyboardEvent& event);
void processMouseEvent(const MouseEvent& event);
void processGamepadEvent(const GamepadEvent& event);

} // namespace click
