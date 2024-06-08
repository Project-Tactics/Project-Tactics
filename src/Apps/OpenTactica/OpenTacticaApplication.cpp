#include "OpenTacticaApplication.h"

#include "Component/RotateItem.h"
#include "Component/RotateAroundPoint.h"

#include "States/DemoState.h"
#include "States/EmptyState.h"
#include "States/LoadState.h"
#include "States/UnloadState.h"
#include "States/MapState.h"

#include <Libs/Utility/Reflection.h>

namespace tactics {

void OpenTacticaApplication::setupComponentReflections() {
	using namespace component;
	defineComponentsReflection<RotateItem, RotateAroundPoint>();
}

std::string OpenTacticaApplication::initialize(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
	fsmBuilder
		.state<LoadState>("Load", serviceLocator)
		.on("proceed").jumpTo("Map00")

		.state<DemoState>("DemoScene", serviceLocator)
		.on("exit").jumpTo("Unload")
		.on("empty").jumpTo("Empty")

		.state<EmptyState>("Empty")
		.on("proceed").jumpTo("Unload")

		.state<UnloadState>("Unload", serviceLocator)
		.on("proceed").exitFsm();

	for (auto mapIndex = 0; mapIndex < 5; ++mapIndex) {
		fsmBuilder
			.state<MapState>(fmt::format("Map{:02d}", mapIndex), serviceLocator, mapIndex)
			.on("exit").jumpTo("Unload")
			.on("next").jumpTo(fmt::format("Map{:02d}", (mapIndex + 1) % 5))
			.on("empty").jumpTo("Empty");
	}

	return "Load";
}

}
