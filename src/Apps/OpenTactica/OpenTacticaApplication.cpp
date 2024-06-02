#include "OpenTacticaApplication.h"

#include "States/DemoState.h"
#include "States/EmptyState.h"
#include "States/LoadState.h"
#include "States/MapState.h"

namespace tactics {

std::string OpenTacticaApplication::initialize(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
	fsmBuilder
		.state<LoadState>("Load", serviceLocator, LoadState::Action::Load)
		.on("proceed").jumpTo("DemoScene")

		.state<DemoState>("DemoScene", serviceLocator)
		.on("exit").jumpTo("Unload")
		.on("alt").jumpTo("Empty")

		.state<EmptyState>("Empty")
		.on("proceed").jumpTo("Unload")

		.state<LoadState>("Unload", serviceLocator, LoadState::Action::Unload)
		.on("proceed").exitFsm();


	for (auto mapIndex = 0; mapIndex < 5; ++mapIndex) {
		fsmBuilder
			.state<MapState>(fmt::format("Map{:02d}", mapIndex), serviceLocator, mapIndex)
			.on("exit").jumpTo("Unload")
			.on("next").jumpTo(fmt::format("Map{:02d}", (mapIndex + 1) % 5))
			.on("alt").jumpTo("Empty");
	}

	return "Load";
}

}
