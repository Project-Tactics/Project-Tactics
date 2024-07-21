#pragma once

#include "ClickEvents.h"
#include "ClickInputTypes.h"

/*
 * "click" is a simple input library which could be extracted from the engine and live as a standalone lib
 */
namespace click {

void initialize(unsigned int maxPlayers, float screenWidth, float screenHeight);
void changeScreenSize(float width, float height);
void update(float deltaTime);
void shutdown();
void setDeviceChangedCallback(DeviceChangedCallback callback, void* userData);

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

bool hasKeyboard(unsigned int index);
bool hasMouse(unsigned int index);
bool hasGamepad(unsigned int index);

const DeviceIdList& gamepads();
const DeviceIdList& mice();
const DeviceIdList& keyboards();
const DeviceIdList& touches();

void holdDevice(PlayerId playerId, DeviceId deviceId);
void releaseDevice(PlayerId playerId, DeviceId deviceId);

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

ActionId createAction(ActionType type, bool normalized);
void destroyAction(ActionId actionId);

const ActionState& actionState(ActionId id, PlayerId playerId);

ActionType type(ActionId id);
unsigned int actions();

/*
 * InputCode
 */

BindingId bind(MapId inputMapId,
			   ActionId actionId,
			   Gesture gesture,
			   std::vector<Trigger> triggers,
			   std::vector<Modifier> modifiers);
void rebind(MapId inputMapId,
			BindingId id,
			Gesture gesture,
			std::vector<Trigger> triggers,
			std::vector<Modifier> modifiers);
void rebind(MapId inputMapId, BindingId id, Gesture gesture, std::vector<Trigger> triggers);
void rebind(MapId inputMapId, BindingId id, Gesture gesture);
void unbind(BindingId id);

/*
 * Event Processing
 */

void processInputEvent(const InputEvent& event);

/*
 * Input Processing
 */
void updateMouse(DeviceId mouseId, float x, float y);
void updateGamepadAxis(DeviceId gamepadId, InputCode axis, const ActionValue& value);

} // namespace click
