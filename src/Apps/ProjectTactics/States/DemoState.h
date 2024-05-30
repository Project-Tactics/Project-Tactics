#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>

#include <entt/entt.hpp>
#include <glm/glm.hpp>

namespace tactics {

class DemoState: public FsmStateWithServices {
public:
	using FsmStateWithServices::FsmStateWithServices;
	FsmAction update() override;
	FsmAction enter() override;
	void exit() override;

private:
	bool onKeyPress(SDL_KeyboardEvent& event) override;
	void _rotateCamera();
	entt::entity _createObject(const glm::vec3& position, std::string_view meshName, std::string_view materialName);

	bool _exitNextFrame{};
	bool _exitNextFrameAlt{};
	float _cameraAngleX{};
	entt::entity _customQuadEntity;
};

}
