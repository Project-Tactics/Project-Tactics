#include "States/LoadState.h"
#include "States/UnloadState.h"

#include <Libs/Fsm/FsmBuilder.h>
#include <Libs/Utility/Service/ServiceLocator.h>

namespace tactics {

class SampleFlow {
public:
	virtual ~SampleFlow() = default;

	SampleFlow(std::string_view sampleName, std::string_view cameraName, std::string_view resourceFileExtension)
		: sampleName(sampleName)
		, cameraName(cameraName)
		, resourceFileExtension(resourceFileExtension) {
		loadState = fmt::format("Load{}", sampleName);
		unloadState = fmt::format("Unload{}", sampleName);
	}

	std::string sampleName;
	std::string cameraName;
	std::string resourceFileExtension;
	std::string loadState;
	std::string unloadState;

	virtual void addFlowToFsm(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) = 0;
};

template<typename TSampleState> struct TSampleFlow : public SampleFlow {
public:
	TSampleFlow(std::string_view sampleName, std::string_view cameraName, std::string_view resourceFileExtension)
		: SampleFlow(sampleName, cameraName, resourceFileExtension) {}

	void addFlowToFsm(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
		auto resourcePackName = fmt::format("demo{}", sampleName);
		auto resourceFile = fmt::format("_{}/resources{}", resourcePackName, resourceFileExtension);

		// clang-format off
		fsmBuilder
			.state<LoadState>(loadState, serviceLocator, resourceFile, HashId(resourcePackName), HashId(cameraName))
				.on("proceed").jumpTo(sampleName)
				.onAppExitRequest().jumpTo(unloadState)

			.state<TSampleState>(sampleName, serviceLocator)
				.on("exit").jumpTo(unloadState)
				.onAppExitRequest().jumpTo(unloadState)

			.state<UnloadState>(unloadState, serviceLocator, HashId(resourcePackName))
				.on("proceed").jumpTo("Selection")
				.onAppExitRequest().exitFsm();
		// clang-format on
	}
};

} // namespace tactics
