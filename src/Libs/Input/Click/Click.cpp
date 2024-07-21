#include "Click.h"

#include "ClickConditions.h"
#include "ClickModifiers.h"

#include <assert.h>

namespace click {

Context* ctx = nullptr;

std::array<DeviceType, static_cast<unsigned int>(InputCode::Count)> inputCodeToDevice = {DeviceType::None};

std::array mouseInputCode = {InputCode::LeftButton,
							 InputCode::RightButton,
							 InputCode::MiddleButton,
							 InputCode::MouseX,
							 InputCode::MouseY,
							 InputCode::MouseZ,
							 InputCode::MouseXY};

std::array gamepadInputCode = {InputCode::ButtonA,
							   InputCode::ButtonB,
							   InputCode::ButtonX,
							   InputCode::ButtonY,
							   InputCode::ButtonBack,
							   InputCode::ButtonGuide,
							   InputCode::ButtonStart,
							   InputCode::ButtonLeftStick,
							   InputCode::ButtonRightStick,
							   InputCode::ButtonLeftShoulder,
							   InputCode::ButtonRightShoulder,
							   InputCode::ButtonDPadUp,
							   InputCode::ButtonDPadDown,
							   InputCode::ButtonDPadRight,
							   InputCode::ButtonDPadLeft,
							   InputCode::ButtonMisc,
							   InputCode::ButtonPaddle1,
							   InputCode::ButtonPaddle2,
							   InputCode::ButtonPaddle3,
							   InputCode::ButtonPaddle4,
							   InputCode::ButtonTouchpad,
							   InputCode::AxisLeftX,
							   InputCode::AxisLeftY,
							   InputCode::AxisLeftXY,
							   InputCode::AxisRightX,
							   InputCode::AxisRightY,
							   InputCode::AxisRightXY,
							   InputCode::AxisTriggerLeft,
							   InputCode::AxisTriggerRight};

std::array touchInputCode = {InputCode::Touch1,
							 InputCode::Touch2,
							 InputCode::Touch3,
							 InputCode::Touch4,
							 InputCode::Touch5,
							 InputCode::Touch6,
							 InputCode::Touch7,
							 InputCode::Touch8,
							 InputCode::Touch9,
							 InputCode::Touch10};

std::array keyboardInputCode = {InputCode::KeyReturn,
								InputCode::KeyEscape,
								InputCode::KeyBackspace,
								InputCode::KeyTab,
								InputCode::KeySpace,
								InputCode::KeyExclaim,
								InputCode::KeyQuotedbl,
								InputCode::KeyHash,
								InputCode::KeyPercent,
								InputCode::KeyDollar,
								InputCode::KeyAmpersand,
								InputCode::KeyQuote,
								InputCode::KeyLeftparen,
								InputCode::KeyRightparen,
								InputCode::KeyAsterisk,
								InputCode::KeyPlus,
								InputCode::KeyComma,
								InputCode::KeyMinus,
								InputCode::KeyPeriod,
								InputCode::KeySlash,
								InputCode::Key0,
								InputCode::Key1,
								InputCode::Key2,
								InputCode::Key3,
								InputCode::Key4,
								InputCode::Key5,
								InputCode::Key6,
								InputCode::Key7,
								InputCode::Key8,
								InputCode::Key9,
								InputCode::KeyColon,
								InputCode::KeySemicolon,
								InputCode::KeyLess,
								InputCode::KeyEquals,
								InputCode::KeyGreater,
								InputCode::KeyQuestion,
								InputCode::KeyAt,
								InputCode::KeyLeftbracket,
								InputCode::KeyBackslash,
								InputCode::KeyRightbracket,
								InputCode::KeyCaret,
								InputCode::KeyUnderscore,
								InputCode::KeyBackquote,
								InputCode::KeyA,
								InputCode::KeyB,
								InputCode::KeyC,
								InputCode::KeyD,
								InputCode::KeyE,
								InputCode::KeyF,
								InputCode::KeyG,
								InputCode::KeyH,
								InputCode::KeyI,
								InputCode::KeyJ,
								InputCode::KeyK,
								InputCode::KeyL,
								InputCode::KeyM,
								InputCode::KeyN,
								InputCode::KeyO,
								InputCode::KeyP,
								InputCode::KeyQ,
								InputCode::KeyR,
								InputCode::KeyS,
								InputCode::KeyT,
								InputCode::KeyU,
								InputCode::KeyV,
								InputCode::KeyW,
								InputCode::KeyX,
								InputCode::KeyY,
								InputCode::KeyZ,
								InputCode::KeyCapslock,
								InputCode::KeyF1,
								InputCode::KeyF2,
								InputCode::KeyF3,
								InputCode::KeyF4,
								InputCode::KeyF5,
								InputCode::KeyF6,
								InputCode::KeyF7,
								InputCode::KeyF8,
								InputCode::KeyF9,
								InputCode::KeyF10,
								InputCode::KeyF11,
								InputCode::KeyF12,
								InputCode::KeyPrintscreen,
								InputCode::KeyScrolllock,
								InputCode::KeyPause,
								InputCode::KeyInsert,
								InputCode::KeyHome,
								InputCode::KeyPageup,
								InputCode::KeyDelete,
								InputCode::KeyEnd,
								InputCode::KeyPagedown,
								InputCode::KeyRight,
								InputCode::KeyLeft,
								InputCode::KeyDown,
								InputCode::KeyUp,
								InputCode::KeyNumlockclear,
								InputCode::KeypadDivide,
								InputCode::KeypadMultiply,
								InputCode::KeypadMinus,
								InputCode::KeypadPlus,
								InputCode::KeypadEnter,
								InputCode::Keypad1,
								InputCode::Keypad2,
								InputCode::Keypad3,
								InputCode::Keypad4,
								InputCode::Keypad5,
								InputCode::Keypad6,
								InputCode::Keypad7,
								InputCode::Keypad8,
								InputCode::Keypad9,
								InputCode::Keypad0,
								InputCode::KeypadPeriod,
								InputCode::KeyApplication,
								InputCode::KeyPower,
								InputCode::KeypadEquals,
								InputCode::KeyF13,
								InputCode::KeyF14,
								InputCode::KeyF15,
								InputCode::KeyF16,
								InputCode::KeyF17,
								InputCode::KeyF18,
								InputCode::KeyF19,
								InputCode::KeyF20,
								InputCode::KeyF21,
								InputCode::KeyF22,
								InputCode::KeyF23,
								InputCode::KeyF24,
								InputCode::KeyExecute,
								InputCode::KeyHelp,
								InputCode::KeyMenu,
								InputCode::KeySelect,
								InputCode::KeyStop,
								InputCode::KeyAgain,
								InputCode::KeyUndo,
								InputCode::KeyCut,
								InputCode::KeyCopy,
								InputCode::KeyPaste,
								InputCode::KeyFind,
								InputCode::KeyMute,
								InputCode::KeyVolumeup,
								InputCode::KeyVolumedown,
								InputCode::KeypadComma,
								InputCode::KeypadEqualsas400,
								InputCode::KeyAlterase,
								InputCode::KeySysreq,
								InputCode::KeyCancel,
								InputCode::KeyClear,
								InputCode::KeyPrior,
								InputCode::KeyReturn2,
								InputCode::KeySeparator,
								InputCode::KeyOut,
								InputCode::KeyOper,
								InputCode::KeyClearagain,
								InputCode::KeyCrsel,
								InputCode::KeyExsel,
								InputCode::Keypad00,
								InputCode::Keypad000,
								InputCode::KeyThousandsseparator,
								InputCode::KeyDecimalseparator,
								InputCode::KeyCurrencyunit,
								InputCode::KeyCurrencysubunit,
								InputCode::KeypadLeftparen,
								InputCode::KeypadRightparen,
								InputCode::KeypadLeftbrace,
								InputCode::KeypadRightbrace,
								InputCode::KeypadTab,
								InputCode::KeypadBackspace,
								InputCode::KeypadA,
								InputCode::KeypadB,
								InputCode::KeypadC,
								InputCode::KeypadD,
								InputCode::KeypadE,
								InputCode::KeypadF,
								InputCode::KeypadXor,
								InputCode::KeypadPower,
								InputCode::KeypadPercent,
								InputCode::KeypadLess,
								InputCode::KeypadGreater,
								InputCode::KeypadAmpersand,
								InputCode::KeypadDblampersand,
								InputCode::KeypadVerticalbar,
								InputCode::KeypadDblverticalbar,
								InputCode::KeypadColon,
								InputCode::KeypadHash,
								InputCode::KeypadSpace,
								InputCode::KeypadAt,
								InputCode::KeypadExclam,
								InputCode::KeypadMemstore,
								InputCode::KeypadMemrecall,
								InputCode::KeypadMemclear,
								InputCode::KeypadMemadd,
								InputCode::KeypadMemsubtract,
								InputCode::KeypadMemmultiply,
								InputCode::KeypadMemdivide,
								InputCode::KeypadPlusminus,
								InputCode::KeypadClear,
								InputCode::KeypadClearentry,
								InputCode::KeypadBinary,
								InputCode::KeypadOctal,
								InputCode::KeypadDecimal,
								InputCode::KeypadHexadecimal,
								InputCode::KeyLctrl,
								InputCode::KeyLshift,
								InputCode::KeyLalt,
								InputCode::KeyLgui,
								InputCode::KeyRctrl,
								InputCode::KeyRshift,
								InputCode::KeyRalt,
								InputCode::KeyRgui,
								InputCode::KeyMode,
								InputCode::KeyAudionext,
								InputCode::KeyAudioprev,
								InputCode::KeyAudiostop,
								InputCode::KeyAudioplay,
								InputCode::KeyAudiomute,
								InputCode::KeyMediaselect,
								InputCode::KeyWww,
								InputCode::KeyMail,
								InputCode::KeyCalculator,
								InputCode::KeyComputer,
								InputCode::KeyAcSearch,
								InputCode::KeyAcHome,
								InputCode::KeyAcBack,
								InputCode::KeyAcForward,
								InputCode::KeyAcStop,
								InputCode::KeyAcRefresh,
								InputCode::KeyAcBookmarks,
								InputCode::KeyBrightnessdown,
								InputCode::KeyBrightnessup,
								InputCode::KeyDisplayswitch,
								InputCode::KeyKbdillumtoggle,
								InputCode::KeyKbdillumdown,
								InputCode::KeyKbdillumup,
								InputCode::KeyEject,
								InputCode::KeySleep,
								InputCode::KeyApp1,
								InputCode::KeyApp2,
								InputCode::KeyAudiorewind,
								InputCode::KeyAudiofastforward,
								InputCode::KeySoftleft,
								InputCode::KeySoftright,
								InputCode::KeyCall,
								InputCode::KeyEndcall};

auto _isValueZero(const ActionValue& value) {
	return value.vec3.x == 0.0f && value.vec3.y == 0.0f && value.vec3.z == 0.0f;
}

float _magnitude(const ActionValue& value) {
	return sqrtf(value.vec3.x * value.vec3.x + value.vec3.y * value.vec3.y + value.vec3.z * value.vec3.z);
}

float _magnitudeSquared(const ActionValue& value) {
	return value.vec3.x * value.vec3.x + value.vec3.y * value.vec3.y + value.vec3.z * value.vec3.z;
}

void _normalize(ActionValue& value) {
	auto magnitude = _magnitude(value);
	if (magnitude == 0.0f) {
		return;
	}
	value.vec3.x /= magnitude;
	value.vec3.y /= magnitude;
	value.vec3.z /= magnitude;
}

/*
 * Player
 */

void clearPlayer(PlayerId playerId) {
	auto& player = ctx->players[playerId];
	player.inputMaps.clear();
	player.heldDevices.clear();
}

unsigned int players() {
	return static_cast<unsigned int>(ctx->players.size());
}

Player& player(PlayerId playerId) {
	return ctx->players[playerId];
}

/*
 * Device
 */

DeviceId _getNextAvailableDeviceId() {
	for (auto index = 0u; index < ctx->devices.devices.size(); ++index) {
		if (ctx->devices.devices[index].type == DeviceType::None) {
			return static_cast<DeviceId>(index);
		}
	}
	assert(false);
	return 0;
}

DeviceId _createDevice(DeviceType type, void* customData) {
	auto nextId = _getNextAvailableDeviceId();
	ctx->devices.devices[nextId] = {type, nextId, customData};
	return nextId;
}

void _notifyDeviceChanged(DeviceId id, DeviceType type, DeviceEvent event) {
	if (ctx->_deviceChanged.callback != nullptr) {
		ctx->_deviceChanged.callback(id, type, event, ctx->_deviceChanged.userData);
	}
}

void _clearInputValuesFor(InputValues& inputValues, DeviceType deviceType) {
	switch (deviceType) {
	case DeviceType::Mouse: {
		for (auto& inputCode : mouseInputCode) {
			inputValues[static_cast<unsigned int>(inputCode)] = {};
		}
		break;
	}
	case DeviceType::Gamepad: {
		for (auto& inputCode : gamepadInputCode) {
			inputValues[static_cast<unsigned int>(inputCode)] = {};
		}
		break;
	}
	case DeviceType::Keyboard: {
		for (auto& inputCode : keyboardInputCode) {
			inputValues[static_cast<unsigned int>(inputCode)] = {};
		}
		break;
	}
	case DeviceType::Touch: {
		for (auto& inputCode : touchInputCode) {
			inputValues[static_cast<unsigned int>(inputCode)] = {};
		}
		break;
	}
	default: break;
	}
}

void _releaseDevice(DeviceType deviceType, DeviceId deviceId) {
	for (auto& player : ctx->players) {
		if (std::erase(player.heldDevices, deviceId) > 0) {
			_clearInputValuesFor(player.inputValues, deviceType);
		}
	}
}

void _installDevice(DeviceIdList& deviceIds, DeviceType type, void* customData) {
	auto id = _createDevice(type, customData);
	deviceIds.push_back(id);
	_notifyDeviceChanged(id, type, DeviceEvent::Added);
}

void installGamepad(void* customData) {
	_installDevice(ctx->devices.gamepads, DeviceType::Gamepad, customData);
}

void installKeyboard() {
	_installDevice(ctx->devices.keyboards, DeviceType::Keyboard, nullptr);
}

void installMouse() {
	_installDevice(ctx->devices.mice, DeviceType::Mouse, nullptr);
}

void _uninstallDevice(DeviceIdList& deviceIds, DeviceType deviceType, DeviceId id) {
	auto& device = ctx->devices.devices[id];
	device.type = DeviceType::None;
	device.customData = nullptr;
	device.deviceId = 0;
	std::erase(deviceIds, id);
	_notifyDeviceChanged(id, deviceType, DeviceEvent::Removed);
	_releaseDevice(deviceType, id);
}

void uninstallMouse(DeviceId id) {
	_uninstallDevice(ctx->devices.mice, DeviceType::Mouse, id);
}

void uninstallKeyboard(DeviceId id) {
	_uninstallDevice(ctx->devices.keyboards, DeviceType::Keyboard, id);
}

void uninstallGamepad(DeviceId id) {
	_uninstallDevice(ctx->devices.gamepads, DeviceType::Gamepad, id);
}

DeviceId keyboard(unsigned int index) {
	return ctx->devices.keyboards[index];
}

DeviceId mouse(unsigned int index) {
	return ctx->devices.mice[index];
}

DeviceId gamepad(unsigned int index) {
	return ctx->devices.gamepads[index];
}

bool hasKeyboard(unsigned int index) {
	return index < ctx->devices.keyboards.size();
}

bool hasMouse(unsigned int index) {
	return index < ctx->devices.mice.size();
}

bool hasGamepad(unsigned int index) {
	return index < ctx->devices.gamepads.size();
}

const DeviceIdList& gamepads() {
	return ctx->devices.gamepads;
}

const DeviceIdList& mice() {
	return ctx->devices.mice;
}

const DeviceIdList& keyboards() {
	return ctx->devices.keyboards;
}

const DeviceIdList& touches() {
	return ctx->devices.touches;
}

DeviceData& device(DeviceId id) {
	return ctx->devices.devices[id];
}

DeviceType deviceType(DeviceId id) {
	return ctx->devices.devices[id].type;
}

void holdDevice(PlayerId playerId, DeviceId deviceId) {
	Player& player = ctx->players[playerId];
	if (std::ranges::find(player.heldDevices, deviceId) != player.heldDevices.end()) {
		return;
	}
	player.heldDevices.emplace_back(deviceId);
}

void releaseDevice(PlayerId playerId, DeviceId deviceId) {
	Player& player = ctx->players[playerId];
	std::erase(player.heldDevices, deviceId);
}

bool isHoldingDevice(PlayerId playerId, DeviceType deviceType) {
	auto& player = ctx->players[playerId];
	for (auto& deviceId : player.heldDevices) {
		if (ctx->devices.devices[deviceId].type == deviceType) {
			return true;
		}
	}
	return false;
}

bool isHoldingDevice(PlayerId playerId, DeviceId deviceId) {
	auto& player = ctx->players[playerId];
	return std::ranges::find(player.heldDevices, deviceId) != player.heldDevices.end();
}

DeviceType getDeviceTypeFromInputCode(InputCode code) {
	return inputCodeToDevice[static_cast<unsigned int>(code)];
}

/*
 * Core
 */

void initialize(unsigned int maxPlayers, float screenWidth, float screenHeight) {
	ctx = new Context{};
	ctx->actions.reserve(32);
	ctx->players.resize(maxPlayers);
	ctx->devices.mice.reserve(8);
	ctx->devices.keyboards.reserve(8);
	ctx->devices.gamepads.reserve(8);
	ctx->devices.touches.reserve(8);
	ctx->devices.devices.fill({DeviceType::None, InvalidDeviceId, nullptr});
	ctx->_screenWidth = screenWidth;
	ctx->_screenHeight = screenHeight;

	for (auto& code : mouseInputCode) {
		inputCodeToDevice[static_cast<unsigned int>(code)] = DeviceType::Mouse;
	}

	for (auto& code : gamepadInputCode) {
		inputCodeToDevice[static_cast<unsigned int>(code)] = DeviceType::Gamepad;
	}

	for (auto& code : touchInputCode) {
		inputCodeToDevice[static_cast<unsigned int>(code)] = DeviceType::Touch;
	}

	for (auto& code : keyboardInputCode) {
		inputCodeToDevice[static_cast<unsigned int>(code)] = DeviceType::Keyboard;
	}
}

void changeScreenSize(float width, float height) {
	ctx->_screenHeight = height;
	ctx->_screenWidth = width;
}

Context& getContext() {
	return *ctx;
}

void shutdown() {
	delete ctx;
	ctx = nullptr;
}

void setDeviceChangedCallback(DeviceChangedCallback callback, void* userData) {
	ctx->_deviceChanged = {callback, userData};
}

ActionValue& _getInputValueFromPlayer(Player& player, InputCode inputCode) {
	return player.inputValues[static_cast<unsigned int>(inputCode)];
}

void _updateBindingValue(Player& player, Binding& binding, GestureSimple& gesture) {
	binding.value = _getInputValueFromPlayer(player, gesture.input);
}

void _updateBindingValue(Player& player, Binding& binding, Gesture2D& gesture) {
	auto& xValue = _getInputValueFromPlayer(player, gesture.x);
	auto& yValue = _getInputValueFromPlayer(player, gesture.y);
	binding.value.vec3.x = xValue.scalar;
	binding.value.vec3.y = yValue.scalar;
}

void _updateBindingValue(Player& player, Binding& binding, Gesture3D& gesture) {
	auto& xValue = _getInputValueFromPlayer(player, gesture.x);
	auto& yValue = _getInputValueFromPlayer(player, gesture.y);
	auto& zValue = _getInputValueFromPlayer(player, gesture.z);
	binding.value.vec3.x = xValue.scalar;
	binding.value.vec3.y = yValue.scalar;
	binding.value.vec3.z = zValue.scalar;
}

void _updateBindingValue(Player& player, Binding& binding, GestureDirectional2D& gesture) {
	auto& leftValue = _getInputValueFromPlayer(player, gesture.left);
	auto& rightValue = _getInputValueFromPlayer(player, gesture.right);
	auto& upValue = _getInputValueFromPlayer(player, gesture.up);
	auto& downValue = _getInputValueFromPlayer(player, gesture.down);
	binding.value.vec3.x = rightValue.scalar - leftValue.scalar;
	binding.value.vec3.y = upValue.scalar - downValue.scalar;
}

void _updateBindingValue(Player& player, Binding& binding, GestureDirectional3D& gesture) {
	auto& leftValue = _getInputValueFromPlayer(player, gesture.left);
	auto& rightValue = _getInputValueFromPlayer(player, gesture.right);
	auto& upValue = _getInputValueFromPlayer(player, gesture.up);
	auto& downValue = _getInputValueFromPlayer(player, gesture.down);
	auto& backValue = _getInputValueFromPlayer(player, gesture.back);
	auto& forwardValue = _getInputValueFromPlayer(player, gesture.forward);
	binding.value.vec3.x = rightValue.scalar - leftValue.scalar;
	binding.value.vec3.y = upValue.scalar - downValue.scalar;
	binding.value.vec3.z = forwardValue.scalar - backValue.scalar;
}

void update(float deltaTime) {
	for (auto& action : ctx->actions) {
		for (auto& state : action.states) {
			state.state = InputState::None;
			state.value = ActionValue{};
		}
	}

	for (auto playerIndex = 0; auto& player : ctx->players) {
		for (auto& inputMap : player.inputMaps) {
			if (!inputMap.isEnabled) {
				continue;
			}
			for (auto& actionMap : inputMap.actionMaps) {
				auto state = InputState::None;
				auto value = ActionValue{};
				for (auto& binding : actionMap.bindings) {
					std::visit([&](auto&& arg) { _updateBindingValue(player, binding, arg); }, binding.gesture);

					for (auto& modifier : binding.modifiers) {
						std::visit([&](auto&& arg) { modify(arg, binding.value); }, modifier);
					}

					auto allConditionsSucceded = true;
					for (auto& condition : binding.conditions) {
						auto conditionState =
							std::visit([&](auto&& arg) { return update(arg, binding, deltaTime); }, condition);
						allConditionsSucceded &= conditionState == ConditionState::Triggered;
					}

					if (allConditionsSucceded) {
						state = InputState::Triggered;
						value.vec3.x += binding.value.vec3.x;
						value.vec3.y += binding.value.vec3.y;
						value.vec3.z += binding.value.vec3.z;
					}
				}

				auto& action = ctx->actions[actionMap.actionId];
				auto& actionState = action.states[playerIndex];
				if (actionState.state == InputState::Triggered) {
					if (action.normalized) {
						_normalize(actionState.value);
					}
					continue;
				}
				actionState.state = state;
				actionState.value = value;
				if (action.normalized) {
					_normalize(actionState.value);
				}
			}
		}
		++playerIndex;
	}
}

/*
 * Mapping
 *
 */

InputMap* _getInputMap(MapId mapId) {
	for (auto& player : ctx->players) {
		for (auto& inputMap : player.inputMaps) {
			if (inputMap.id == mapId) {
				return &inputMap;
			}
		}
	}
	return nullptr;
}

MapId addInputMap(PlayerId playerId) {
	static MapId mapId = 0;
	auto& player = ctx->players[playerId];
	player.inputMaps.emplace_back(mapId);
	return mapId++;
}

void removeInputMap(MapId mapId) {
	for (auto& player : ctx->players) {
		std::erase_if(player.inputMaps, [mapId](const InputMap& inputMap) { return inputMap.id == mapId; });
	}
}

void disableInputMap(MapId mapId) {
	auto inputMap = _getInputMap(mapId);
	assert(inputMap != nullptr);
	inputMap->isEnabled = false;
}

void enableInputMap(MapId mapId) {
	auto inputMap = _getInputMap(mapId);
	assert(inputMap != nullptr);
	inputMap->isEnabled = true;
}

/*
 * Action
 */

ActionId createAction(ActionType type, bool normalized) {
	ActionId actionId = 0;
	if (ctx->_freeActionIndices.empty()) {
		actionId = static_cast<unsigned int>(ctx->actions.size());
		ctx->actions.emplace_back();
	} else {
		actionId = ctx->_freeActionIndices.back();
		ctx->_freeActionIndices.pop_back();
	}
	auto& action = ctx->actions[actionId];
	action.type = type;
	action.normalized = normalized;
	action.states.resize(ctx->players.size());
	return actionId++;
}

void destroyAction(ActionId actionId) {
	ctx->_freeActionIndices.push_back(actionId);
	ctx->actions[actionId] = {};

	for (auto& player : ctx->players) {
		for (auto& inputMap : player.inputMaps) {
			std::erase_if(inputMap.actionMaps,
						  [actionId](const ActionMapping& actionMap) { return actionMap.actionId == actionId; });
		}
	}
}

ActionType type(ActionId id) {
	return ctx->actions[id].type;
}

unsigned int actions() {
	return static_cast<unsigned int>(ctx->actions.size());
}

const ActionState& actionState(ActionId actionId, PlayerId playerId) {
	auto& state = ctx->actions[actionId].states[playerId];
	return state;
}

/*
 * InputCode
 */

BindingId _bind(InputMap& inputMap,
				ActionId actionId,
				Gesture gesture,
				std::vector<Condition> conditions,
				std::vector<Modifier> modifiers) {
	auto itr = std::ranges::find_if(inputMap.actionMaps,
									[actionId](const ActionMapping& item) { return item.actionId == actionId; });
	if (itr == inputMap.actionMaps.end()) {
		inputMap.actionMaps.emplace_back(actionId);
		itr = inputMap.actionMaps.end() - 1;
	}

	static BindingId id = 0;
	itr->bindings.emplace_back(id, std::move(gesture), std::move(conditions), std::move(modifiers));
	return id++;
}

Binding* _getBinding(InputMap* inputMap, BindingId id) {
	for (auto& actionMap : inputMap->actionMaps) {
		for (auto& binding : actionMap.bindings) {
			if (binding.id == id) {
				return &binding;
			}
		}
	}
	return nullptr;
}

Binding* _getBinding(MapId inputMapId, BindingId id) {
	auto inputMap = _getInputMap(inputMapId);
	assert(inputMap != nullptr);
	return _getBinding(inputMap, id);
}

BindingId bind(MapId inputMapId,
			   ActionId actionId,
			   Gesture gesture,
			   std::vector<Condition> conditions,
			   std::vector<Modifier> modifiers) {
	auto inputMap = _getInputMap(inputMapId);
	assert(inputMap != nullptr);
	return _bind(*inputMap, actionId, std::move(gesture), std::move(conditions), std::move(modifiers));
}

void rebind(MapId inputMapId,
			BindingId id,
			Gesture gesture,
			std::vector<Condition> conditions,
			std::vector<Modifier> modifiers) {
	auto binding = _getBinding(inputMapId, id);
	assert(binding != nullptr);
	binding->gesture = std::move(gesture);
	binding->conditions = std::move(conditions);
	binding->modifiers = std::move(modifiers);
	binding->value = ActionValue{};
}

void rebind(MapId inputMapId, BindingId id, Gesture gesture, std::vector<Condition> conditions) {
	auto binding = _getBinding(inputMapId, id);
	assert(binding != nullptr);
	binding->gesture = std::move(gesture);
	binding->conditions = std::move(conditions);
	binding->value = ActionValue{};
}

void rebind(MapId inputMapId, BindingId id, Gesture gesture) {
	auto binding = _getBinding(inputMapId, id);
	assert(binding != nullptr);
	binding->gesture = std::move(gesture);
	binding->value = ActionValue{};
}

void unbind(BindingId id) {
	for (auto& player : ctx->players) {
		for (auto& inputMap : player.inputMaps) {
			for (auto& actionMap : inputMap.actionMaps) {
				if (std::erase_if(actionMap.bindings, [id](auto& binding) { return binding.id == id; }) > 0) {
					return;
				}
			}
		}
	}
}

/*
 * Input & Event Processing
 */

void updateMouse(DeviceId mouseId, float x, float y) {
	x /= ctx->_screenWidth;
	y /= ctx->_screenHeight;
	auto xRel = x - ctx->mouseState.x;
	auto yRel = y - ctx->mouseState.y;
	ctx->mouseState.x = x;
	ctx->mouseState.y = y;

	if (xRel == ctx->mouseState.xRel && yRel == ctx->mouseState.yRel) {
		return;
	}

	ctx->mouseState.xRel = xRel;
	ctx->mouseState.yRel = yRel;

	processInputEvent({InputCode::MouseX, mouseId, xRel});
	processInputEvent({InputCode::MouseY, mouseId, yRel});
	processInputEvent({InputCode::MouseXY, mouseId, {.vec2 = {xRel, yRel}}});
}

void updateGamepadAxis(DeviceId gamepadId, InputCode axis, const ActionValue& value) {
	processInputEvent({axis, gamepadId, value});
}

void processInputEvent(const InputEvent& event) {
	for (auto& player : ctx->players) {
		if (player.heldDevices.empty() ||
			std::ranges::find(player.heldDevices, event.deviceId) == player.heldDevices.end()) {
			continue;
		}

		player.inputValues[static_cast<unsigned int>(event.inputCode)] = event.value;
	}
}

const ActionValue& inputValue(InputCode code, PlayerId playerId) {
	return ctx->players[playerId].inputValues[static_cast<unsigned int>(code)];
}

} // namespace click
