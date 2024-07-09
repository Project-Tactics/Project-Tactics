#pragma once

#include "ClickEvents.h"
#include "ClickInputTypes.h"

/*
 * "click" is a simple input library which could be extracted from the engine and live as a standalone lib
 */
namespace click {

void initialize(unsigned int maxPlayers);
void update(float deltaTime);

/*
 * Player
 */

void clearPlayer(PlayerId playerId);
unsigned int players();

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

void holdDevice(PlayerId playerId, DeviceId deviceId);
void releaseDevice(PlayerId playerId, DeviceId deviceId);

/*
 * Trigger Creation
 */
Trigger downTrigger(float actuationThreshold);
Trigger pressTrigger(float actuationThreshold);
Trigger releaseTrigger(float actuationThreshold);
Trigger holdTrigger(float holdTime);
Trigger continuousTrigger(float actuationThreshold);

/*
 * Modifier Creation
 */
Modifier negateModifier();
Modifier toAxisModifier(Axis axis);

/*
 * Mapping
 */

MapId addInputMap(PlayerId playerId);
void removeInputMap(MapId mapId);
void disableInputMap(MapId mapId);
void enableInputMap(MapId mapId);

/*
 * Action
 */

ActionId createAction(ActionType type);
void destroyAction(ActionId actionId);

std::tuple<InputState, bool> impulseState(ActionId id, PlayerId playerId);
std::tuple<InputState, float> axis1DState(ActionId id, PlayerId playerId);
std::tuple<InputState, Axis2D> axis2DState(ActionId id, PlayerId playerId);
std::tuple<InputState, Axis3D> axis3DState(ActionId id, PlayerId playerId);

ActionType type(ActionId id);

/*
 * Gesture
 */

GestureId bindGesture(MapId inputMapId,
					  ActionId actionId,
					  Gesture gesture,
					  std::vector<Trigger> triggers,
					  std::vector<Modifier> modifiers);
void rebindGesture(MapId inputMapId,
				   GestureId gestureId,
				   Gesture gesture,
				   std::vector<Trigger> triggers,
				   std::vector<Modifier> modifiers);
void rebindGesture(MapId inputMapId, GestureId gestureId, Gesture gesture, std::vector<Trigger> triggers);
void rebindGesture(MapId inputMapId, GestureId gestureId, Gesture gesture);
void unbindGesture(GestureId gestureId);

/*
 * Event Processing
 */

void processInputEvent(const InputEvent& event);

/*
 * Input Processing
 */
void updateMouse(DeviceId mouseId, float x, float y);
void updateGamepadAxis(DeviceId gamepadId, Gesture axis, ActionValue& value);

} // namespace click
