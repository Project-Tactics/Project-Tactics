#include "OpenTacticaPrototypeApplication.h"

#include "Component/CharacterComponent.h"
#include "DataSet/CharacterDataSet.h"
#include "State/GameState.h"
#include "State/InitializationState.h"
#include "State/UninitializationState.h"

namespace tactics {

void OpenTacticaPrototypeApplication::setupComponentReflections() {
	// Components
	using namespace component;
	defineReflection<CharName, CharStats, ChargeTime, CharBattleReady, CharAvatar>();

	// DataSets
	defineReflection<CharacterDataSet>();
}

HashId OpenTacticaPrototypeApplication::initialize(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
	// clang-format off
    fsmBuilder
        .state<InitializationState>("Load", serviceLocator)
            .on("proceed").jumpTo("Game")
            .onAppExitRequest().jumpTo("Unload")

        .state<GameState>("Game", serviceLocator)
            .on("exit").jumpTo("Unload")
            .onAppExitRequest().jumpTo("Unload")

        .state<UninitializationState>("Unload", serviceLocator)
            .on("proceed").exitFsm();
	// clang-format on

	return "Load"_id;
}

} // namespace tactics
