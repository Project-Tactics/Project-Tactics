#include "SamplesApplication.h"

#include "Component/BattleCamera.h"
#include "Component/CharacterFacing.h"
#include "Component/FreeCamera.h"
#include "Component/RotateAroundPoint.h"
#include "Component/RotateItem.h"
#include "States/DemoMapState.h"
#include "States/DemoSimpleState.h"
#include "States/DemoSpriteState.h"
#include "States/EmptyState.h"
#include "States/LoadState.h"
#include "States/UnloadState.h"

#include <Libs/Resource/IniFile/IniFile.h>
#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Utility/Reflection.h>

namespace tactics {

void OpenTacticaApplication::setupComponentReflections() {
	using namespace component;
	defineComponentsReflection<BattleCamera, CharacterFacing, RotateItem, RotateAroundPoint, FreeCamera>();
}

HashId OpenTacticaApplication::initialize(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
	auto configFile =
		serviceLocator.getService<resource::ResourceSystem>().getResource<resource::IniFile>("devUserConfigFile"_id);
	auto state = configFile->getOrCreate("demo", "fsm", std::string("map"));

	if (state == "sprite") {
		return _initializeSpriteDemo(serviceLocator, fsmBuilder);
	} else if (state == "map") {
		return _initializeMapDemo(serviceLocator, fsmBuilder);
	} else {
		return _initializeSimpleDemo(serviceLocator, fsmBuilder);
	}
}

HashId OpenTacticaApplication::_initializeSpriteDemo(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
	// clang-format off
	fsmBuilder
		.state<LoadState>("Load", serviceLocator, "_demoSprites/resources.json", "demoSprites"_id, "spriteCamera"_id)
		.on("proceed").jumpTo("Sprites")
		.onAppExitRequest().jumpTo("Unload")

		.state<DemoSpriteState>("Sprites", serviceLocator)
		.on("exit").jumpTo("Unload")
		.onAppExitRequest().jumpTo("Unload")

		.state<UnloadState>("Unload", serviceLocator, "demoSprites"_id)
		.on("proceed").exitFsm()
		.onAppExitRequest().exitFsm();
	// clang-format on
	return "Load"_id;
}

HashId OpenTacticaApplication::_initializeMapDemo(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
	// clang-format off
	fsmBuilder
		.state<LoadState>("Load", serviceLocator, "_demoMaps/resources.lua", "demoMaps"_id, "mapCamera"_id)
		.on("proceed").jumpTo("Map")
		.onAppExitRequest().jumpTo("Unload")

		.state<DemoMapState>("Map", serviceLocator)
		.on("exit").jumpTo("Unload")
		.onAppExitRequest().jumpTo("Unload")

		.state<UnloadState>("Unload", serviceLocator, "demoMaps"_id)
		.on("proceed").exitFsm()
		.onAppExitRequest().exitFsm();
	// clang-format on
	return "Load"_id;
}

HashId OpenTacticaApplication::_initializeSimpleDemo(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
	// clang-format off
	fsmBuilder
		.state<LoadState>("Load", serviceLocator, "_demoSimple/resources.json", "demoSimple"_id, "rotateAroundCamera"_id)
		.on("proceed").jumpTo("DemoScene")

		.state<DemoSimpleState>("DemoScene", serviceLocator)
		.on("exit").jumpTo("Unload")
		.on("empty").jumpTo("Empty")
		.onAppExitRequest().jumpTo("Unload")

		.state<EmptyState>("Empty")
		.on("proceed").jumpTo("Unload")
		.onAppExitRequest().jumpTo("Unload")

		.state<UnloadState>("Unload", serviceLocator, "demoSimple"_id)
		.on("proceed").exitFsm()
		.onAppExitRequest().exitFsm();
	// clang-format on
	return "Load"_id;
}

} // namespace tactics
