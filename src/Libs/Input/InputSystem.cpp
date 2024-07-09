#include "InputSystem.h"

#include "Click/Backend/ClickSDLBackend.h"

#include <Libs/Resource/IniFile/IniFile.h>
#include <Libs/Utility/HashId.h>
#include <Libs/Utility/Log/Log.h>
#include <Libs/Utility/Time/EngineTime.h>

namespace tactics {

InputSystem::InputSystem(std::shared_ptr<resource::IniFile> configFile) {
	LOG_TRACE(Log::Input, "Initialize click library");
	click::initialize(configFile->get("Engine", "players", 1));
	click::initSdlBackend();

	_moveActionId = click::createAction(click::ActionType::Axis1D);
	click::destroyAction(_moveActionId);
	_moveActionId = click::createAction(click::ActionType::Axis2D);

	click::GestureId gestureToRemap1{};
	click::GestureId gestureToRemap2{};
	click::MapId mapToRemap1{};
	for (uint8_t i = 0; i < 1; ++i) {
		auto inputMap = click::addInputMap(i);

		auto gesture1 = click::bindGesture(inputMap,
										   _moveActionId,
										   click::Gesture::KeyW,
										   {click::downTrigger(0.5f)},
										   {click::toAxisModifier(click::Axis::YZX)});

		auto gesture2 = click::bindGesture(inputMap,
										   _moveActionId,
										   click::Gesture::KeyS,
										   {click::downTrigger(0.5f)},
										   {click::toAxisModifier(click::Axis::YZX), click::negateModifier()});

		click::unbindGesture(gesture2);
		gesture2 = click::bindGesture(inputMap,
									  _moveActionId,
									  click::Gesture::KeyS,
									  {click::downTrigger(0.5f)},
									  {click::toAxisModifier(click::Axis::YZX), click::negateModifier()});

		click::bindGesture(inputMap, _moveActionId, click::Gesture::KeyD, {click::downTrigger(0.5f)}, {});
		auto negate = click::negateModifier();
		click::bindGesture(inputMap, _moveActionId, click::Gesture::KeyA, {click::downTrigger(0.5f)}, {negate});

		if (i == 1) {
			gestureToRemap1 = gesture1;
			gestureToRemap2 = gesture2;
			mapToRemap1 = inputMap;
		}

		// click::disableInputMap(inputMap);
	}

	// Remapping
	// click::rebindGesture(mapToRemap1, gestureToRemap1, click::Gesture::KeyUp);
	// click::rebindGesture(mapToRemap1, gestureToRemap2, click::Gesture::KeyDown);

	click::holdDevice(0, click::keyboard(0));
	// click::holdDevice(1, click::keyboard(0));

	_spaceActionId = click::createAction(click::ActionType::Axis1D);
	_mouseActionId = click::createAction(click::ActionType::Axis2D);

	auto downTrigger = click::downTrigger(0.5f);
	auto pressTrigger = click::pressTrigger(0.5f);
	auto releaseTrigger = click::releaseTrigger(0.5f);

	auto inputMap = click::addInputMap(0);

	click::bindGesture(inputMap, _spaceActionId, click::Gesture::KeySpace, {pressTrigger}, {});
	click::bindGesture(inputMap, _spaceActionId, click::Gesture::KeySpace, {releaseTrigger}, {});

	auto continuous = click::continuousTrigger(0.1f);
	click::bindGesture(inputMap, _mouseActionId, click::Gesture::MouseX, {continuous}, {});
	continuous = click::continuousTrigger(0.5f);
	click::bindGesture(inputMap, _moveActionId, click::Gesture::AxisLeftXY, {continuous}, {});

	click::holdDevice(0, click::mouse(0));
	click::holdDevice(0, click::gamepad(0));

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
	std::vector<std::tuple<click::ActionId, const char*>> actions = {
		std::make_tuple(_moveActionId, "MOVE"),
		std::make_tuple(_mouseActionId, "MOUSE"),
		std::make_tuple(_spaceActionId, "SPACE"),
	};

	click::updateSdlBackend();
	click::update(EngineTime::fixedDeltaTime<float>());

	for (click::PlayerId index = 0; index < click::players(); ++index) {
		for (auto& [actionId, name] : actions) {
			if (click::type(actionId) == click::ActionType::Axis1D) {
				auto [state, value] = click::axis1DState(actionId, index);
				std::string message;
				if (state == click::InputState::Triggered) {
					LOG_INFO(Log::Input, "Player {} - {}: {}", index, name, value);
				}
			} else if (click::type(actionId) == click::ActionType::Axis2D) {
				auto [state, value] = click::axis2DState(actionId, index);
				std::string message;
				if (state == click::InputState::Triggered) {
					LOG_INFO(Log::Input, "Player {} - {}: {}, {}", index, name, value.x, value.y);
				}
			} else if (click::type(actionId) == click::ActionType::Axis3D) {
				auto [state, value] = click::axis3DState(actionId, index);
				std::string message;
				if (state == click::InputState::Triggered) {
					LOG_INFO(Log::Input, "Player {} - {}: {}, {}, {}", index, name, value.x, value.y, value.z);
				}
			}
		}
	}
}
} // namespace tactics
