#pragma once

#include "../SamplesUtils.h"

#include <Libs/Fsm/FsmStateWithServices.h>

#include <glm/glm.hpp>

namespace tactics {

class DemoSpriteState : public SampleState {
public:
	using SampleState::SampleState;
	FsmAction update() override;
	FsmAction enter() override;
	void exit() override;

private:
	void _createCharacters(const glm::vec3& offset, int count);
};

} // namespace tactics
