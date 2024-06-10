#include "OpenTacticaApplication.h"

#include "Component/RotateItem.h"
#include "Component/RotateAroundPoint.h"

#include "States/DemoSimpleState.h"
#include "States/EmptyState.h"
#include "States/LoadState.h"
#include "States/DemoMapState.h"
#include "States/DemoSpriteState.h"
#include "States/UnloadState.h"

#include <Libs/Utility/Reflection.h>

namespace tactics {

void OpenTacticaApplication::setupComponentReflections() {
	using namespace component;
	defineComponentsReflection<RotateItem, RotateAroundPoint>();
}

std::string OpenTacticaApplication::initialize(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
	enum class FsmToRun {
		SpriteDemo,
		MapDemo,
		SimpleDemo
	};

	auto fsmToRun = FsmToRun::SpriteDemo;

	switch (fsmToRun) {
	case FsmToRun::SpriteDemo:
		return _initializeSpriteDemo(serviceLocator, fsmBuilder);
	case FsmToRun::MapDemo:
		return _initializeMapDemo(serviceLocator, fsmBuilder);
	case FsmToRun::SimpleDemo:
	default:
		return _initializeSimpleDemo(serviceLocator, fsmBuilder);
	}
}

std::string OpenTacticaApplication::_initializeSpriteDemo(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
	fsmBuilder
		.state<LoadState>("Load", serviceLocator, "_demoSprites/resources.json", "demoSprites", "simpleCamera")
		.on("proceed").jumpTo("Sprites")

		.state<DemoSpriteState>("Sprites", serviceLocator)
		.on("exit").jumpTo("Unload")

		.state<UnloadState>("Unload", serviceLocator, "demoSprites")
		.on("proceed").exitFsm();

	return "Load";
}

std::string OpenTacticaApplication::_initializeMapDemo(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
	fsmBuilder
		.state<LoadState>("Load", serviceLocator, "_demoMaps/resources.lua", "demoMaps", "mapCamera")
		.on("proceed").jumpTo("Map")

		.state<DemoMapState>("Map", serviceLocator)
		.on("exit").jumpTo("Unload")

		.state<UnloadState>("Unload", serviceLocator, "demoMaps")
		.on("proceed").exitFsm();

	return "Load";
}

std::string OpenTacticaApplication::_initializeSimpleDemo(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
	fsmBuilder
		.state<LoadState>("Load", serviceLocator, "_demoSimple/resources.json", "demoSimple", "rotateAroundCamera")
		.on("proceed").jumpTo("DemoScene")

		.state<DemoSimpleState>("DemoScene", serviceLocator)
		.on("exit").jumpTo("Unload")
		.on("empty").jumpTo("Empty")

		.state<EmptyState>("Empty")
		.on("proceed").jumpTo("Unload")

		.state<UnloadState>("Unload", serviceLocator, "demoSimple")
		.on("proceed").exitFsm();

	return "Load";
}

}
