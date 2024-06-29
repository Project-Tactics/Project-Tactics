#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>

#include <Libs/Utility/HashString.h>

namespace tactics {

class LoadState: public FsmStateWithServices {
public:
	LoadState(ServiceLocator& services, const std::string& resourceDefinitionPath, const hash_string& resourcePackName, const hash_string& cameraPrefab);
	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;

private:
	void _loadResources();
	void _createViewport();
	void _createCamera();
	void _setupRenderSteps();

	std::string _resourceDefinitionPath;
	hash_string _resourcePackName;
	hash_string _cameraPrefab;
};

}
