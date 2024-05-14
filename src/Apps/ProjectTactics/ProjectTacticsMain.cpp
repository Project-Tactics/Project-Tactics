#include <iostream>

#include <Engine/Application/Application.h>

int main(int /*argc*/, char** /*argv*/) {

	auto application = std::make_unique<tactics::Application>();
	application->run();

	return 0;
}
