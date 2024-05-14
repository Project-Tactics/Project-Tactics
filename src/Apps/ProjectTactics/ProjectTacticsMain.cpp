#include <iostream>

#include <Engine/Application/Application.h>

int main(int argc, char** argv) {
	using namespace tactics::engine;

	auto application = std::make_unique<Application>();
	application->run();

	return 0;
}
