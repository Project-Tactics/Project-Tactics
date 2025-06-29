#include "GameState.h"

#include "../Component/CharacterComponent.h"
#include "../DataSet/CharacterDataSet.h"
#include "../Overlay/PrototypeOverlay.h"

#include <Engine/Scene/SceneSystem.h>

#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/Input/InputSystem.h>
#include <Libs/Overlay/OverlaySystem.h>
#include <Libs/Resource/DataSet/DataSetSystem.h>
#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Utility/Time/EngineTime.h>

namespace tactics {

void createCharacterFromData(const CharacterDataSet& charData, SceneSystem& sceneSystem, const glm::vec3& position) {
	auto entity = sceneSystem.createEntity(HashId(charData.name), "character"_id);
	entity.getComponent<component::CharName>().name = charData.name;
	entity.getComponent<component::CharAvatar>().textureId = charData.avatarId;
	auto& charStats = entity.getComponent<component::CharStats>();
	charStats.hp = charData.hp;
	charStats.maxHp = charData.hp;
	charStats.mp = charData.mp;
	charStats.maxMp = charData.mp;
	charStats.level = charData.level;
	charStats.xp = charData.xp;
	charStats.move = charData.move;
	charStats.jump = charData.jump;
	auto& chargeTime = entity.getComponent<component::ChargeTime>();
	chargeTime.chargeTime = 0.0f;
	chargeTime.speed = charData.speed;

	entity.getComponent<component::Transform>().setPosition(position);
}

FsmAction GameState::enter() {
	auto& sceneSystem = getService<SceneSystem>();
	auto& dataSetSystem = getService<resource::DataSetSystem>();
	auto characterDataSet = dataSetSystem.getDataSet<CharacterDataSet>("main_characters"_id);
	std::vector<glm::vec3> positions;
	auto x = 0.0f;
	std::transform(characterDataSet->data.begin(),
				   characterDataSet->data.end(),
				   std::back_inserter(positions),
				   [&x](const CharacterDataSet&) {
					   x += 1;
					   return glm::vec3{x, 0, 0};
				   });

	auto i = 0;
	for (auto& charData : characterDataSet->data) {
		createCharacterFromData(charData, sceneSystem, positions[i]);
		++i;
	}

	sceneSystem.createEntity("MainCamera"_id, "simpleCamera"_id);

	auto& overlaySystem = getService<OverlaySystem>();
	auto& resourceSystem = getService<resource::ResourceSystem>();
	overlaySystem.addOverlay<PrototypeOverlay>("PrototypeOverlay",
											   false,
											   getService<RenderSystem>(),
											   getService<EntityComponentSystem>(),
											   resourceSystem);

	return FsmAction::none();
}

void GameState::exit() {
	auto& sceneSystem = getService<SceneSystem>();
	sceneSystem.clearScene();

	auto& overlaySystem = getService<OverlaySystem>();
	overlaySystem.removeOverlay("PrototypeOverlay");
}

FsmAction GameState::update() {
	auto& inputSystem = getService<InputSystem>();
	if (inputSystem.checkAction("anyKeyPressed")) {
		return FsmAction::transition("exit"_id);
	}

	auto& registry = getService<SceneSystem>().getRegistry();

	float deltaTime = EngineTime::fixedDeltaTime<float>();
	component::BattleSystem::advanceTick(registry, deltaTime);

	return FsmAction::none();
}

} // namespace tactics
