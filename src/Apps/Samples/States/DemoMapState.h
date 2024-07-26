#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>

namespace tactics {

class DemoMapState : public FsmStateWithServices {
public:
	using FsmStateWithServices::FsmStateWithServices;
	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;

private:
	void _createScene();
	void _setupInput();

	unsigned int _mapIndex{};
};

} // namespace tactics
