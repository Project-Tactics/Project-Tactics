#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>

#include <glm/glm.hpp>

namespace tactics {

class DemoSpriteState : public FsmStateWithServices {
public:
	using FsmStateWithServices::FsmStateWithServices;
	FsmAction update() override;
	FsmAction enter() override;
	void exit() override;

private:
	FsmEventAction onKeyPress(SDL_KeyboardEvent& event) override;
	void _createCharacters(const glm::vec3& offset, int count);
};

} // namespace tactics
