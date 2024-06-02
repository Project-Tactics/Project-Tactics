#include "OpenTacticaApplication.h"

#include "States/DemoState.h"
#include "States/EmptyState.h"
#include "States/LoadState.h"
#include "States/MapState.h"

namespace tactics {

std::string OpenTacticaApplication::initialize(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
	fsmBuilder
		.state<LoadState>("Load", serviceLocator, LoadState::Action::Load)
		.on("proceed").jumpTo("Map")

		.state<DemoState>("DemoScene", serviceLocator)
		.on("exit").jumpTo("Unload")
		.on("alt").jumpTo("Empty")

		.state<MapState>("Map", serviceLocator)
		.on("exit").jumpTo("Unload")
		.on("alt").jumpTo("Empty")

		.state<EmptyState>("Empty")
		.on("proceed").jumpTo("Unload")

		.state<LoadState>("Unload", serviceLocator, LoadState::Action::Unload)
		.on("proceed").exitFsm();

	return "Load";
}

}
