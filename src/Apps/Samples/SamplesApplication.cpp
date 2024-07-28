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

#include <Libs/Resource/IniFile/IniFile.h>
#include <Libs/Resource/ResourceSystem.h>
#include <Libs/Utility/Reflection.h>

namespace tactics {

SamplesApplication::SamplesApplication() {
	_addSampleFlow<DemoSpriteState>("sprite", "Sprite", "spriteCamera");
	_addSampleFlow<DemoMapState>("map", "Map", "mapCamera", ".lua");
	_addSampleFlow<DemoParticlesState>("particles", "Particles", "particlesCamera");
	_addSampleFlow<DemoSimpleState>("simple", "Simple", "rotateAroundCamera");
	// ADD SAMPLES HERE
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
	auto defaultState = HashId(configFile->getOrCreate("demo", "fsm", std::string("none")));

	// clang-format off
	fsmBuilder
		.state<SampleSelectionState>("Selection", serviceLocator)
			.on("exit").exitFsm()
			.onAppExitRequest().exitFsm();
	
	for (auto& [sampleName, sampleFlow] : _sampleFlows) {
		fsmBuilder.on(sampleFlow->sampleName).jumpTo(sampleFlow->loadState);
	}
	// clang-format on

	auto startingState = "Selection"_id;
	for (auto& [sampleKey, sampleFlow] : _sampleFlows) {
		sampleFlow->addFlowToFsm(serviceLocator, fsmBuilder);
		if (sampleKey == defaultState) {
			startingState = HashId(fmt::format("Load{}", sampleFlow->sampleName));
		}
	}

	return startingState;
}

} // namespace tactics
