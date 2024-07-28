#pragma once

#include "../SamplesUtils.h"

namespace tactics {

class DemoMapState : public SampleState {
public:
	using SampleState::SampleState;
	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;

private:
	void _createScene();
	void _setupInput();

	unsigned int _mapIndex{};
};

} // namespace tactics
