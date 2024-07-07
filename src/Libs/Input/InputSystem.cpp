#include "InputSystem.h"

#include "Click/Backend/ClickSDLBackend.h"

#include <Libs/Utility/HashId.h>
#include <Libs/Utility/Log/Log.h>
#include <Libs/Utility/Time/EngineTime.h>

namespace tactics {

InputSystem::InputSystem() {
	LOG_TRACE(Log::Input, "Initialize click library");
	click::initializeContext();
	click::initSdlBackend();

	auto player = click::owner();
	(void)player;

	auto keyboardDeviceId = click::keyboard(0);
	// auto keyboardDeviceId2 = click::keyboard_copy(0);

	auto mouseDeviceId = click::mouse(0);
	auto gamepadDeviceId = click::gamepad(0);

	// click::assign(player, keyboardDeviceId);
	//  click::assign(player, mouseDeviceId);
	//  click::assign(player, gamepadDeviceId);

	// click::assign(player2, keyboardDeviceId2);

	_spaceActionId = click::axis1D();
	_moveActionId = click::axis1D();
	_mouseActionId = click::axis1D();

	auto downTrigger = click::downTrigger(0.5f);
	auto pressTrigger = click::pressTrigger(0.5f);
	auto releaseTrigger = click::releaseTrigger(0.5f);
	click::map(_spaceActionId, keyboardDeviceId, {.key = click::Key::KeySpace}, {pressTrigger}, {});
	click::map(_spaceActionId, keyboardDeviceId, {.key = click::Key::KeySpace}, {releaseTrigger}, {});

	click::map(_mouseActionId, mouseDeviceId, {.mouse = click::MouseGesture::MouseY}, {}, {});

	auto negateModifier = click::negateModifier();
	click::map(_moveActionId, keyboardDeviceId, {.key = click::Key::KeyW}, {downTrigger}, {});
	click::map(_moveActionId, keyboardDeviceId, {.key = click::Key::KeyS}, {downTrigger}, {negateModifier});

	auto downTrigger2 = click::downTrigger(0.1f);
	click::map(_moveActionId, gamepadDeviceId, {.gamepad = click::GamepadGesture::AxisLeftY}, {downTrigger2}, {});

	// clang-format off
	/*
	using namespace click;
	actionBuilder
		.context("default")
		.player(0)
		.action<ActionType::Axis1D>("Move"_id)
			.gesture<Key::KeyW>(0)
				.trigger<TriggerType::Down>(0.5f)
			.gesture<Key::KeyS>(0)
				.trigger<TriggerType::Down>(0.5f)
				.modifier<ModifierType::Negate>()
			.gesture<GamepadGesture::AxisLeftY>(0)
				.trigger<TriggerType::Down>(0.1f);

	actionBuilder.build();
	*/
	// clang-format on
}

void InputSystem::processEvents(SDL_Event& event) {
	click::processSdlEvents(event);
}

void InputSystem::update() {
	std::array<std::tuple<click::InputActionId, const char*>, 3> actions = {
		std::make_tuple(_moveActionId, "MOVE"),
		std::make_tuple(_mouseActionId, "MOUSE"),
		std::make_tuple(_spaceActionId, "SPACE"),
	};

	for (auto& [actionId, name] : actions) {
		if (click::type(actionId) == click::ActionType::Axis1D) {
			auto [state, value] = click::axis1DState(actionId);
			std::string message;
			if (state == click::InputState::Triggered) {
				LOG_INFO(Log::Input, "{}: {}", name, value);
			}
		} else if (click::type(actionId) == click::ActionType::Axis2D) {
			auto [state, value] = click::axis2DState(actionId);
			std::string message;
			if (state == click::InputState::Triggered) {
				LOG_INFO(Log::Input, "{}: {}, {}", name, value.x, value.y);
			}
		} else if (click::type(actionId) == click::ActionType::Axis3D) {
			auto [state, value] = click::axis3DState(actionId);
			std::string message;
			if (state == click::InputState::Triggered) {
				LOG_INFO(Log::Input, "{}: {}, {}, {}", name, value.x, value.y, value.z);
			}
		}
	}

	click::update(EngineTime::fixedDeltaTime<float>());
}
} // namespace tactics
