#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>

namespace tactics {

class LoadState: public FsmStateWithServices {
public:
	LoadState(ServiceLocator& services, const std::string& resourceDefinitionPath, const std::string& resourcePackName, const std::string& cameraPrefab);
	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;

private:
	void _loadResources();
	void _createViewport();
	void _createCamera();
	void _setupRenderSteps();

	std::string _resourceDefinitionPath;
	std::string _resourcePackName;
	std::string _cameraPrefab;
};

}
