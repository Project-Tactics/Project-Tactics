#pragma once

#include "ClickEnumTypes.h"

#include <array>
#include <tuple>
#include <variant>
#include <vector>

namespace click {

using ActionId = unsigned int;
using DeviceId = uint8_t;
using PlayerId = uint8_t;
using MapId = uint8_t;
using BindingId = uint16_t;

using DeviceChangedCallback = void (*)(DeviceId id, DeviceType type, DeviceEvent event, void* userData);

const auto InvalidDeviceId = std::numeric_limits<uint8_t>::max();

struct Vec3 {
	float x{};
	float y{};
	float z{};
};

struct Vec2 {
	float x{};
	float y{};
};

union ActionValue {
	float scalar;
	Vec2 vec2;
	Vec3 vec3{};
};

float _magnitude(const ActionValue& value);
float _magnitudeSquared(const ActionValue& value);
void _normalize(ActionValue& value);

struct ActionState {
	InputState state{};
	ActionValue value{};
};

struct InputAction {
	ActionType type{};
	bool normalized{};
	std::vector<ActionState> states;
};

struct DownCondition {
	float actuationThreshold{};
};

struct PressCondition {
	float actuationThreshold{};
	ConditionState state{};
};

struct ReleaseCondition {
	float actuationThreshold{};
	ConditionState state{};
};

struct HoldCondition {
	float actuationThreshold{};
	float holdTime{};
	float currentTime{};
	ConditionState state{};
};

struct ContinuousCondition {
	float actuationThreshold{};
	ConditionState state{};
};

using Condition = std::variant<DownCondition, PressCondition, ReleaseCondition, HoldCondition, ContinuousCondition>;

struct NegateModifier {
	Axis axis{};
};

using Modifier = std::variant<NegateModifier>;

struct GestureSimple {
	InputCode input{};
};

struct Gesture2D {
	InputCode x{};
	InputCode y{};
};

struct Gesture3D {
	InputCode x{};
	InputCode y{};
	InputCode z{};
};

struct GestureDirectional2D {
	InputCode left{};
	InputCode right{};
	InputCode down{};
	InputCode up{};
};

struct GestureDirectional3D {
	InputCode left{};
	InputCode right{};
	InputCode down{};
	InputCode up{};
	InputCode back{};
	InputCode forward{};
};

using Gesture = std::variant<GestureSimple, Gesture2D, Gesture3D, GestureDirectional2D, GestureDirectional3D>;

struct MouseState {
	float x{};
	float y{};
	float xRel{};
	float yRel{};
};

struct DeviceData {
	DeviceType type{DeviceType::None};
	DeviceId deviceId{InvalidDeviceId};
	void* customData{};
};

using DeviceList = std::array<DeviceData, 16>;
using DeviceIdList = std::vector<DeviceId>;

struct DeviceInfo {
	DeviceIdList mice;
	DeviceIdList keyboards;
	DeviceIdList gamepads;
	DeviceIdList touches;

	DeviceList devices;
	std::vector<int> _freeDeviceIndices;
};

struct Binding {
	BindingId id;
	Gesture gesture;
	std::vector<Condition> conditions;
	std::vector<Modifier> modifiers;
	ActionValue value{};
};

struct ActionMapping {
	ActionId actionId{};
	std::vector<Binding> bindings;
};

struct InputMap {
	MapId id{};
	std::vector<ActionMapping> actionMaps;
	bool isEnabled{true};
};

using InputValues = std::array<ActionValue, static_cast<unsigned int>(InputCode::Count)>;

struct Player {
	std::vector<InputMap> inputMaps;
	std::vector<DeviceId> heldDevices;
	InputValues inputValues;
};

struct DeviceChanged {
	DeviceChangedCallback callback{};
	void* userData{};
};

struct Context {
	std::vector<InputAction> actions;
	std::vector<Player> players;
	DeviceInfo devices;
	MouseState mouseState;
	DeviceChanged _deviceChanged;
	std::vector<int> _freeActionIndices;
	float _screenWidth{};
	float _screenHeight{};
};

} // namespace click
