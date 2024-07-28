#include "SampleSelectionState.h"

#include "../Overlay/SampleSelectionOverlay.h"

#include <Libs/Input/InputSystem.h>
#include <Libs/Overlay/OverlaySystem.h>
#include <Libs/Rendering/RenderSystem.h>

namespace tactics {

const char* overlayName = "SampleSelection";

SampleSelectionState::SampleSelectionState(ServiceLocator& services,
										   HashId transitionToTrigger,
										   const SampleFlows& sampleFlows)
	: FsmStateWithServices(services)
	, _transitionToTrigger(transitionToTrigger)
	, _sampleFlows(sampleFlows) {}

FsmAction SampleSelectionState::enter() {
	auto& overlaySystem = getService<OverlaySystem>();
	auto& renderSystem = getService<RenderSystem>();
	overlaySystem.addOverlay<SampleSelectionOverlay>(
		overlayName,
		true,
		renderSystem,
		_sampleFlows,
		[this](const std::string& selectedSample) { _transitionToTrigger = HashId(selectedSample); });

	if (!_transitionToTrigger.isEmpty()) {
		return FsmAction::transition(std::exchange(_transitionToTrigger, HashId::none));
	}

	return FsmAction::none();
}

void SampleSelectionState::exit() {
	auto& overlaySystem = getService<OverlaySystem>();
	overlaySystem.removeOverlay(overlayName);
}

FsmAction SampleSelectionState::update() {
	auto& inputSystem = getService<InputSystem>();
	if (inputSystem.checkAction("exitFromState")) {
		return FsmAction::transition("exit"_id);
	}

	if (!_transitionToTrigger.isEmpty()) {
		return FsmAction::transition(std::exchange(_transitionToTrigger, HashId::none));
	}

	return FsmAction::none();
}

} // namespace tactics
