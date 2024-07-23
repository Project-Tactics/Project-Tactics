#pragma once

#include <Engine/Core/Application.h>

namespace tactics {

class ##APP_NAME##Application : public tactics::Application {
public:
	void setupComponentReflections() override;
	HashId initialize(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) override;
};

} // namespace tactics

