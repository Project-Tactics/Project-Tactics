#pragma once

#include "../SamplesUtils.h"

namespace tactics {

class DemoParticlesState : public SampleState {
public:
	using SampleState::SampleState;
	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;
};

} // namespace tactics
