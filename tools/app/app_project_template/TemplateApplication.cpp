#include "##APP_NAME##Application.h"
#include "States/##APP_NAME##State.h"

namespace tactics {

void ##APP_NAME##Application::setupComponentReflections() {}

HashId ##APP_NAME##Application::initialize(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
	// clang-format off
    fsmBuilder
        .state<##APP_NAME##State>("Default", serviceLocator)
        .on("proceed").exitFsm()
        .onAppExitRequest().exitFsm();
	// clang-format on

	return "Default"_id;
}

} // namespace tactics
