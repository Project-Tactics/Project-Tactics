#include "##APP_PROJECT_NAME##Application.h"

#include <Engine/Core/Engine.h>

int main(int /*argc*/, char** /*argv*/) {
	using namespace tactics;
	Engine::run<##APP_PROJECT_NAME##Application>();
	return 0;
}
