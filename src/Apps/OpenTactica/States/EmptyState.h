#pragma once

#include <Libs/Fsm/FsmState.h>

namespace tactics {

class EmptyState : public FsmState {
public:
	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;

private:
	FsmEventAction onKeyPress(SDL_KeyboardEvent& event) override;
};

} // namespace tactics
