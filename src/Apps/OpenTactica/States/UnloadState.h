#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>
#include <Libs/Utility/HashId.h>

namespace tactics {

class UnloadState : public FsmStateWithServices {
public:
	UnloadState(ServiceLocator& services, const HashId& packageName);
	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;

private:
	HashId _packageName;
};

} // namespace tactics
