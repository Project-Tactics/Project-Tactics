#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>
#include <Libs/Utility/HashId.h>

namespace tactics {

class SetupState : public FsmStateWithServices {
public:
	SetupState(ServiceLocator& services);
	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;

private:
	void _setupRenderSteps();
	void _setupInputMap();
};

} // namespace tactics
