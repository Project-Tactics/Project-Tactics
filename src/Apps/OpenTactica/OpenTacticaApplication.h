#pragma once

#include <Engine/Core/Application.h>

namespace tactics {

class OpenTacticaApplication: public tactics::Application {
public:
	void setupComponentReflections() override;
	std::string initialize(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) override;
};

}

