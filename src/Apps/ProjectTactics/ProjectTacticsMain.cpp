#include "BattlegroundApplication.h"

#include <Engine/Core/Engine.h>

int main(int /*argc*/, char** /*argv*/) {
	using namespace tactics;
	Engine::run<BattlegroundApplication>();
	return 0;
}
