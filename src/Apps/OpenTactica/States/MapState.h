#pragma once

#include <Libs/Fsm/FsmStateWithServices.h>

#include <Engine/Scene/SceneSystem.h>

namespace tactics {

class MapState: public FsmStateWithServices {
public:
	using FsmStateWithServices::FsmStateWithServices;
	FsmAction enter() override;
	FsmAction update() override;
	void exit() override;

private:
	bool onKeyPress(SDL_KeyboardEvent& event) override;

	Entity _createTerrainObject(const glm::vec3& position, std::string_view textureName);
	Entity _createCharacterObject();

	bool _exitNextFrame{};
};

}
