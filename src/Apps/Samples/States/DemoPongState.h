#pragma once

#include "../Component/PongMatch.h"
#include "../SamplesUtils.h"

#include <string>

namespace tactics {

class DemoPongState : public SampleState {
public:
	using SampleState::SampleState;
	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;

private:
	void _createScoreDisplay(const std::string& name, component::PlayerSide side, float positionX);
};

} // namespace tactics
