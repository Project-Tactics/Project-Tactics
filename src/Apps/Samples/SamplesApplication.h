#pragma once

#include <Engine/Core/Application.h>

namespace tactics {

class SamplesApplication : public tactics::Application {
public:
	void setupComponentReflections() override;
	HashId initialize(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) override;

private:
	HashId _initializeSpriteDemo(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder);
	HashId _initializeMapDemo(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder);
	HashId _initializeSimpleDemo(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder);
};

} // namespace tactics

