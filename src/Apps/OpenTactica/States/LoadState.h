#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>

#include <Libs/Utility/HashId.h>

namespace tactics {

class LoadState: public FsmStateWithServices {
public:
	LoadState(ServiceLocator& services, const std::string& resourceDefinitionPath, const HashId& resourcePackName, const HashId& cameraPrefab);
	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;

private:
	void _loadResources();
	void _createViewport();
	void _createCamera();
	void _setupRenderSteps();

	std::string _resourceDefinitionPath;
	HashId _resourcePackName;
	HashId _cameraPrefab;
};

}
