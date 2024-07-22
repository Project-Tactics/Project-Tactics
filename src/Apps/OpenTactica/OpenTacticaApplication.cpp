#include "OpenTacticaApplication.h"

#include "States/EmptyState.h"

namespace tactics {

void OpenTacticaApplication::setupComponentReflections() {}

HashId OpenTacticaApplication::initialize(ServiceLocator&, FsmBuilder& fsmBuilder) {
	// clang-format off
    fsmBuilder
        .state<EmptyState>("Empty")
        .on("proceed").exitFsm()
        .onAppExitRequest().exitFsm();
	// clang-format on

	return "Empty"_id;
}

} // namespace tactics
