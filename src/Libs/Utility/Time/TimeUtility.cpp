#include "TimeUtility.h"

#include <chrono>

namespace tactics {

double TimeUtility::nowInSeconds() {
	auto now = std::chrono::high_resolution_clock::now();
	auto duration = now.time_since_epoch();
	auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	return milliseconds / 1000.0;
}

}
