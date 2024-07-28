#pragma once

#include "States/LoadState.h"
#include "States/UnloadState.h"

#include <Engine/Scene/SceneSystem.h>

#include <Libs/Fsm/FsmBuilder.h>
#include <Libs/Utility/Service/ServiceLocator.h>

namespace tactics {

class SampleState : public FsmStateWithServices {
public:
	using FsmStateWithServices::FsmStateWithServices;

	FsmAction enter() override {
		return FsmAction::none();
	}

	FsmAction update() override {
		return FsmAction::none();
	}

	void exit() override {}

protected:
	void _createCamera(HashId cameraPrefab) {
		auto& sceneSystem = getService<SceneSystem>();
		sceneSystem.createEntity("camera"_id, cameraPrefab);
	}
};

class SampleFlow {
public:
	virtual ~SampleFlow() = default;

	SampleFlow(std::string_view sampleName, std::string_view resourceFileExtension)
		: sampleName(sampleName)
		, resourceFileExtension(resourceFileExtension) {
		loadState = fmt::format("Load{}", sampleName);
		unloadState = fmt::format("Unload{}", sampleName);
	}

	std::string sampleName;
	std::string resourceFileExtension;
	std::string loadState;
	std::string unloadState;

	virtual void addFlowToFsm(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) = 0;
};

template<typename TSampleState> struct TSampleFlow : public SampleFlow {
public:
	TSampleFlow(std::string_view sampleName, std::string_view resourceFileExtension)
		: SampleFlow(sampleName, resourceFileExtension) {}

	void addFlowToFsm(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
		auto resourcePackName = fmt::format("demo{}", sampleName);
		auto resourceFile = fmt::format("_{}/resources{}", resourcePackName, resourceFileExtension);

		// clang-format off
		fsmBuilder
			.state<LoadState>(loadState, serviceLocator, resourceFile, HashId(resourcePackName))
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

using SampleFlows = std::unordered_map<std::string, std::unique_ptr<SampleFlow>>;

} // namespace tactics
