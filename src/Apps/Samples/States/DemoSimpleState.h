#pragma once

#include "../SamplesUtils.h"

namespace tactics {

class DemoSimpleState : public SampleState {
public:
	using SampleState::SampleState;
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
	void _createParticleEffect();
	void _setupInputMap();
};

} // namespace tactics
