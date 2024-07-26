#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>
#include <Libs/Utility/HashId.h>

namespace tactics {

class LoadState : public FsmStateWithServices {
public:
	LoadState(ServiceLocator& services,
			  std::string resourceDefinitionPath,
			  const HashId& resourcePackName,
			  const HashId& cameraPrefab);
	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;

private:
	void _loadResources();
	void _createViewport();
	void _createCamera();
	void _setupRenderSteps();
	void _setupInputMap();

	std::string _resourceDefinitionPath;
	HashId _resourcePackName;
	HashId _cameraPrefab;
};

} // namespace tactics
