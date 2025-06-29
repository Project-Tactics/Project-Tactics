#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>
#include <Libs/Utility/HashId.h>

namespace tactics {

class UnloadState : public FsmStateWithServices {
public:
	UnloadState(ServiceLocator& services, const HashId& packageName, bool destroyRenderQueue = false);
	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;

private:
	HashId _packageName;
	bool _destroyRenderQueue{};
};

} // namespace tactics
