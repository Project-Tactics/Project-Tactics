#include "##APP_NAME##Application.h"

#include "States/EmptyState.h"

namespace tactics {

void ##APP_NAME##Application::setupComponentReflections() {}

HashId ##APP_NAME##Application::initialize(ServiceLocator&, FsmBuilder& fsmBuilder) {
	// clang-format off
    fsmBuilder
        .state<EmptyState>("Empty")
        .on("proceed").exitFsm()
        .onAppExitRequest().exitFsm();
	// clang-format on

	return "Empty"_id;
}

} // namespace tactics
