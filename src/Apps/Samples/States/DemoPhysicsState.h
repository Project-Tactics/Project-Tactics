#pragma once

#include "../SamplesUtils.h"

namespace tactics {

class DemoPhysicsState : public SampleState {
public:
	DemoPhysicsState(ServiceLocator& serviceLocator);
	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;
};

} // namespace tactics
