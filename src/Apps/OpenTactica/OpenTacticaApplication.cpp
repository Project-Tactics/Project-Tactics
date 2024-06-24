#include "OpenTacticaApplication.h"

#include "Component/BattleCamera.h"
#include "Component/CharacterFacing.h"
#include "Component/RotateItem.h"
#include "Component/RotateAroundPoint.h"

#include "States/DemoSimpleState.h"
#include "States/EmptyState.h"
#include "States/LoadState.h"
#include "States/DemoMapState.h"
#include "States/DemoSpriteState.h"
#include "States/UnloadState.h"

#include <Libs/Utility/Reflection.h>
#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Resource/IniFile/IniFile.h>

namespace tactics {

void OpenTacticaApplication::setupComponentReflections() {
	using namespace component;
	defineComponentsReflection<BattleCamera, CharacterFacing, RotateItem, RotateAroundPoint>();
}

std::string OpenTacticaApplication::initialize(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
	auto configFile = serviceLocator.getService<resource::ResourceSystem>().getResource<resource::IniFile>("devUserConfigFile");
	auto state = configFile->getOrCreate("demo", "fsm", std::string("map"));

	if (state == "sprite") {
		return _initializeSpriteDemo(serviceLocator, fsmBuilder);
	} else if (state == "map") {
		return _initializeMapDemo(serviceLocator, fsmBuilder);
	} else {
		return _initializeSimpleDemo(serviceLocator, fsmBuilder);
	}
}

std::string OpenTacticaApplication::_initializeSpriteDemo(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
	fsmBuilder
		.state<LoadState>("Load", serviceLocator, "_demoSprites/resources.json", "demoSprites", "spriteCamera")
		.on("proceed").jumpTo("Sprites")
		.onAppExitRequest().jumpTo("Unload")

		.state<DemoSpriteState>("Sprites", serviceLocator)
		.on("exit").jumpTo("Unload")
		.onAppExitRequest().jumpTo("Unload")

		.state<UnloadState>("Unload", serviceLocator, "demoSprites")
		.on("proceed").exitFsm()
		.onAppExitRequest().exitFsm();

	return "Load";
}

std::string OpenTacticaApplication::_initializeMapDemo(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
	fsmBuilder
		.state<LoadState>("Load", serviceLocator, "_demoMaps/resources.lua", "demoMaps", "mapCamera")
		.on("proceed").jumpTo("Map")
		.onAppExitRequest().jumpTo("Unload")

		.state<DemoMapState>("Map", serviceLocator)
		.on("exit").jumpTo("Unload")
		.onAppExitRequest().jumpTo("Unload")

		.state<UnloadState>("Unload", serviceLocator, "demoMaps")
		.on("proceed").exitFsm()
		.onAppExitRequest().exitFsm();

	return "Load";
}

std::string OpenTacticaApplication::_initializeSimpleDemo(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
	fsmBuilder
		.state<LoadState>("Load", serviceLocator, "_demoSimple/resources.json", "demoSimple", "rotateAroundCamera")
		.on("proceed").jumpTo("DemoScene")

		.state<DemoSimpleState>("DemoScene", serviceLocator)
		.on("exit").jumpTo("Unload")
		.on("empty").jumpTo("Empty")
		.onAppExitRequest().jumpTo("Unload")

		.state<EmptyState>("Empty")
		.on("proceed").jumpTo("Unload")
		.onAppExitRequest().jumpTo("Unload")

		.state<UnloadState>("Unload", serviceLocator, "demoSimple")
		.on("proceed").exitFsm()
		.onAppExitRequest().exitFsm();

	return "Load";
}

}
