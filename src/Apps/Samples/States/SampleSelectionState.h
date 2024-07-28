#pragma once

#include "../SamplesUtils.h"

#include <Libs/Fsm/FsmStateWithServices.h>

namespace tactics {

class SampleSelectionState : public FsmStateWithServices {
public:
	SampleSelectionState(ServiceLocator& services, HashId transitionToTrigger, const SampleFlows& sampleFlows);
	FsmAction enter() override;
	void exit() override;
	FsmAction update() override;

private:
	HashId _transitionToTrigger;
	const SampleFlows& _sampleFlows;
};

} // namespace tactics
