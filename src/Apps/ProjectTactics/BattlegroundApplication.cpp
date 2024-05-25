#include "BattlegroundApplication.h"

#include "States/StartState.h"
#include "States/DemoState.h"

namespace tactics {

std::string BattlegroundApplication::initialize(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
	fsmBuilder
		.state<StartState>("Start", serviceLocator)
		.on("proceed").jumpTo("Map")

		.state<DemoState>("Map", serviceLocator)
		.on("exit").exitFsm();

	return "Start";
}

}
