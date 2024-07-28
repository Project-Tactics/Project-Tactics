#include "SamplesApplication.h"

#include "Component/BattleCamera.h"
#include "Component/CharacterFacing.h"
#include "Component/FreeCamera.h"
#include "Component/RotateAroundPoint.h"
#include "Component/RotateItem.h"
#include "States/DemoMapState.h"
#include "States/DemoParticlesState.h"
#include "States/DemoSimpleState.h"
#include "States/DemoSpriteState.h"
#include "States/EmptyState.h"
#include "States/SampleSelectionState.h"
#include "States/SetupState.h"
// ADD INCLUDE SAMPLES HERE

#include <Libs/Resource/IniFile/IniFile.h>
#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Utility/Reflection.h>

namespace tactics {

SamplesApplication::SamplesApplication() {
	_addSampleFlow<DemoSpriteState>("sprite", "Sprite");
	_addSampleFlow<DemoMapState>("map", "Map", ".lua");
	_addSampleFlow<DemoParticlesState>("particles", "Particles");
	_addSampleFlow<DemoSimpleState>("simple", "Simple");
	// ADD FLOW SAMPLES HERE
}

void SamplesApplication::setupComponentReflections() {
	using namespace component;
	defineComponentsReflection<BattleCamera,
							   BattleCameraInput,
							   CharacterFacing,
							   RotateItem,
							   RotateAroundPoint,
							   FreeCamera>();
}

HashId SamplesApplication::initialize(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) {
	auto& resourceSystem = serviceLocator.getService<resource::ResourceSystem>();
	auto configFile = resourceSystem.getResource<resource::IniFile>("devUserConfigFile"_id);
	auto defaultState = configFile->getOrCreate("demo", "fsm", std::string("none"));

	auto transitionToTrigger = HashId::none;
	if (_sampleFlows.contains(defaultState)) {
		transitionToTrigger = HashId(defaultState);
	}

	// clang-format off
	fsmBuilder
		.state<LoadState>("LoadCommon", serviceLocator, "common/resources.json", "common"_id)
            .on("proceed").jumpTo("Setup")
		.state<SetupState>("Setup", serviceLocator)
            .on("proceed").jumpTo("Selection")
		.state<UnloadState>("Unload", serviceLocator, "common"_id)
            .on("proceed").exitFsm()
		.state<SampleSelectionState>("Selection", serviceLocator, transitionToTrigger, _sampleFlows)
			.on("exit").jumpTo("Unload")
			.onAppExitRequest().jumpTo("Unload");
	
	for (auto& [sampleKey, sampleFlow] : _sampleFlows) {
		fsmBuilder.on(sampleKey).jumpTo(sampleFlow->loadState);
	}
	// clang-format on

	for (auto& [sampleKey, sampleFlow] : _sampleFlows) {
		sampleFlow->addFlowToFsm(serviceLocator, fsmBuilder);
	}

	return "LoadCommon"_id;
}

} // namespace tactics
