#pragma once

#include <Engine/Core/Application.h>

namespace tactics {

class BattlegroundApplication: public tactics::Application {
public:
	std::string initialize(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) override;
};

}

