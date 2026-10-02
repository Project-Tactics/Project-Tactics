#include <Libs/Input/InputSystem.h>
#include <Libs/Resource/IniFile/IniFile.h>
#include <Libs/Resource/Input/InputMap.h>
#include <Libs/Resource/ResourceProvider.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

namespace tactics {
namespace {

class InMemoryIniFileHandle : public FileHandle<ini::IniFile> {
public:
	bool exists() const override {
		return true;
	}

	void save() override {}

	void load() override {}
};

class MockInputResourceProvider : public resource::ResourceProvider {
public:
	MOCK_METHOD(std::shared_ptr<resource::BaseResource>,
				getResource,
				(resource::ResourceType, const HashId&),
				(const override));
	MOCK_METHOD(std::shared_ptr<resource::BaseResource>,
				getResource,
				(resource::ResourceType, resource::ResourceId),
				(const override));
	MOCK_METHOD(resource::BaseResourceManager&, getManager, (resource::ResourceType), (const override));
	MOCK_METHOD(resource::BaseResourceManager&, getManager, (resource::ResourceType), (override));
};

class InputSystemTest : public testing::Test {
protected:
	void SetUp() override {
		auto config = std::make_shared<resource::IniFile>();
		config->fileHandle = std::make_unique<InMemoryIniFileHandle>();
		config->set("Engine", "players", 2);
		_input = std::make_unique<InputSystem>(config, _resources, glm::vec2{640.0f, 480.0f});
		_input->assignDevice(click::DeviceType::Keyboard, 0, 0);
		_input->assignDevice(click::DeviceType::Keyboard, 0, 1);
	}

	std::shared_ptr<resource::InputAction> createAction() {
		auto action = std::make_shared<resource::InputAction>();
		action->actionId = click::createAction(click::ActionType::Scalar, false);
		action->states.resize(_input->getNumPlayers());
		return action;
	}

	void
	addBinding(resource::InputMap& map, const std::shared_ptr<resource::InputAction>& action, click::InputCode key) {
		map.bindings.push_back({action, click::GestureSimple{key}, {click::DownCondition{0.5f}}, {}});
	}

	void pressKey(SDL_Keycode key) {
		SDL_Event event{};
		event.type = SDL_KEYDOWN;
		event.key.keysym.sym = key;
		_input->processEvents(event);
	}

	void updateActions() {
		// Synthetic keyboard events exercise map behavior without polling SDL or creating a window.
		click::update(1.0f / 60.0f);
	}

	void expectAction(const resource::InputAction& action,
					  click::InputState state,
					  float value,
					  click::PlayerId playerId = 0) {
		const auto& actual = _input->getActionState(action.actionId, playerId);
		EXPECT_EQ(actual.state, state);
		EXPECT_FLOAT_EQ(actual.value.scalar, value);
	}

	testing::StrictMock<MockInputResourceProvider> _resources;
	std::unique_ptr<InputSystem> _input;
};

TEST_F(InputSystemTest, UnassignRemovesTheWholeMapAndStopsAllItsBindings) {
	auto firstAction = createAction();
	auto secondAction = createAction();
	auto map = std::make_shared<resource::InputMap>();
	addBinding(*map, firstAction, click::InputCode::KeyA);
	addBinding(*map, firstAction, click::InputCode::KeyB);
	addBinding(*map, secondAction, click::InputCode::KeyC);
	_input->assignInputMap(map);
	pressKey(SDLK_a);
	pressKey(SDLK_b);
	pressKey(SDLK_c);
	updateActions();
	expectAction(*firstAction, click::InputState::Triggered, 2.0f);
	expectAction(*secondAction, click::InputState::Triggered, 1.0f);

	_input->unassignInputMap(map);
	EXPECT_TRUE(_input->getPlayer(0).inputMaps.empty());
	updateActions();
	expectAction(*firstAction, click::InputState::None, 0.0f);
	expectAction(*secondAction, click::InputState::None, 0.0f);
}

TEST_F(InputSystemTest, UnassignByNamePreservesOtherMapsWhenMapAndBindingIdsOverlap) {
	auto otherAction = createAction();
	auto otherMap = std::make_shared<resource::InputMap>();
	addBinding(*otherMap, otherAction, click::InputCode::KeyA);
	addBinding(*otherMap, otherAction, click::InputCode::KeyB);
	_input->assignInputMap(otherMap, 0);

	auto targetAction = createAction();
	auto targetMap = std::make_shared<resource::InputMap>();
	addBinding(*targetMap, targetAction, click::InputCode::KeyA);
	addBinding(*targetMap, targetAction, click::InputCode::KeyB);
	EXPECT_CALL(_resources, getResource(resource::ResourceType::InputMap, "targetMap"_id))
		.Times(2)
		.WillRepeatedly(testing::Return(targetMap));
	_input->assignInputMap("targetMap", 1);
	ASSERT_NE(otherMap->mapId, targetMap->mapId);

	// Force a collision across ID types without depending on Click's persistent allocation counters.
	auto& otherBindings = click::player(0).inputMaps.front().actionMaps.front().bindings;
	auto& targetBindings = click::player(1).inputMaps.front().actionMaps.front().bindings;
	otherBindings[0].id = targetMap->mapId;
	otherBindings[1].id = static_cast<click::BindingId>(targetMap->mapId + 1);
	targetBindings[0].id = static_cast<click::BindingId>(targetMap->mapId + 2);
	targetBindings[1].id = static_cast<click::BindingId>(targetMap->mapId + 3);
	pressKey(SDLK_a);
	pressKey(SDLK_b);
	updateActions();
	expectAction(*otherAction, click::InputState::Triggered, 2.0f, 0);
	expectAction(*targetAction, click::InputState::Triggered, 2.0f, 1);

	_input->unassignInputMap("targetMap");
	EXPECT_TRUE(_input->getPlayer(1).inputMaps.empty());
	ASSERT_EQ(_input->getPlayer(0).inputMaps.size(), 1u);
	EXPECT_EQ(_input->getPlayer(0).inputMaps.front().id, otherMap->mapId);
	EXPECT_EQ(_input->getPlayer(0).inputMaps.front().actionMaps.front().bindings.size(), 2u);
	updateActions();
	expectAction(*otherAction, click::InputState::Triggered, 2.0f, 0);
	expectAction(*targetAction, click::InputState::None, 0.0f, 1);
}

TEST_F(InputSystemTest, EmptyMapAndRepeatedUnassignmentPreserveOtherMaps) {
	auto otherAction = createAction();
	auto otherMap = std::make_shared<resource::InputMap>();
	addBinding(*otherMap, otherAction, click::InputCode::KeyA);
	_input->assignInputMap(otherMap);
	auto emptyMap = std::make_shared<resource::InputMap>();
	_input->assignInputMap(emptyMap);
	ASSERT_EQ(_input->getPlayer(0).inputMaps.size(), 2u);
	pressKey(SDLK_a);
	updateActions();
	expectAction(*otherAction, click::InputState::Triggered, 1.0f);

	_input->unassignInputMap(emptyMap);
	_input->unassignInputMap(emptyMap);
	ASSERT_EQ(_input->getPlayer(0).inputMaps.size(), 1u);
	EXPECT_EQ(_input->getPlayer(0).inputMaps.front().id, otherMap->mapId);
	updateActions();
	expectAction(*otherAction, click::InputState::Triggered, 1.0f);
}

TEST_F(InputSystemTest, ReassignmentAfterUnassignmentRestoresInput) {
	auto action = createAction();
	auto map = std::make_shared<resource::InputMap>();
	addBinding(*map, action, click::InputCode::KeyA);
	_input->assignInputMap(map);
	pressKey(SDLK_a);
	updateActions();
	expectAction(*action, click::InputState::Triggered, 1.0f);

	_input->unassignInputMap(map);
	updateActions();
	expectAction(*action, click::InputState::None, 0.0f);

	_input->assignInputMap(map);
	ASSERT_EQ(_input->getPlayer(0).inputMaps.size(), 1u);
	updateActions();
	expectAction(*action, click::InputState::Triggered, 1.0f);
}

} // namespace
} // namespace tactics
