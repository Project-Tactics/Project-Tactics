#pragma once

#include <Engine/Core/Application.h>

namespace tactics {

class OpenTacticaApplication: public tactics::Application {
public:
	void setupComponentReflections() override;
	std::string initialize(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) override;

private:
	std::string _initializeSpriteDemo(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder);
	std::string _initializeMapDemo(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder);
	std::string _initializeSimpleDemo(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder);
};

}

