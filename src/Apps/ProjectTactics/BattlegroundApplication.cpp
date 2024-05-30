#include "BattlegroundApplication.h"

#include "States/EmptyState.h"
#include "States/DemoState.h"
#include "States/StartState.h"

namespace tactics {

std::string BattlegroundApplication::initialize(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
	fsmBuilder
		.state<StartState>("Start", serviceLocator)
		.on("proceed").jumpTo("Map")

		.state<DemoState>("Map", serviceLocator)
		.on("exit").exitFsm()
		.on("alt").jumpTo("Empty")

		.state<EmptyState>("Empty")
		.on("proceed").exitFsm();

	return "Start";
}

}
