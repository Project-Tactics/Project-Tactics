#include "OpenTacticaPrototypeApplication.h"

#include <Engine/Core/Engine.h>

int main(int /*argc*/, char** /*argv*/) {
	using namespace tactics;
	Engine::run<OpenTacticaPrototypeApplication>();
	return 0;
}
