#include "OpenTacticaApplication.h"

#include "States/DefaultState.h"

namespace tactics {

void OpenTacticaApplication::setupComponentReflections() {}

HashId OpenTacticaApplication::initialize(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
	// clang-format off
    fsmBuilder
        .state<DefaultState>("Empty", serviceLocator)
        .on("proceed").exitFsm()
        .onAppExitRequest().exitFsm();
	// clang-format on

	return "Empty"_id;
}

} // namespace tactics
