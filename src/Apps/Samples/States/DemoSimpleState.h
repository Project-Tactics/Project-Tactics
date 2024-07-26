#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>

namespace tactics {

class DemoSimpleState : public FsmStateWithServices {
public:
	using FsmStateWithServices::FsmStateWithServices;
	FsmAction update() override;
	FsmAction enter() override;
	void exit() override;

private:
	void _createPlane();
	void _createTeapot();
	void _createCrate();
	void _createQuads();
	void _createExtraRotatingQuads();
	void _createCustomQuadWithCustomResources();
	void _setupInputMap();
};

} // namespace tactics
