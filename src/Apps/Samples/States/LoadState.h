#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>
#include <Libs/Utility/HashId.h>

namespace tactics {

class LoadState : public FsmStateWithServices {
public:
	LoadState(ServiceLocator& services, std::string resourceDefinitionPath, const HashId& resourcePackName);
	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;

private:
	void _loadResources();

	std::string _resourceDefinitionPath;
	HashId _resourcePackName;
};

} // namespace tactics
