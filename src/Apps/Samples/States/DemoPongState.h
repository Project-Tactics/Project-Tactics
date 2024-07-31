#pragma once

#include "../SamplesUtils.h"

#include <Libs/Ecs/Entity.h>

namespace tactics {

class DemoPongState : public SampleState {
public:
	using SampleState::SampleState;
	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;

private:
	Entity _playerLeft;
	Entity _playerRight;
	Entity _ball;
	Entity _scoreLeft;
};

} // namespace tactics
