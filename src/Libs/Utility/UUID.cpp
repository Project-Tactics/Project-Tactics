#include <thread>

uint64_t generateUUID() {
	static std::atomic<uint64_t> globalCounter(0);
	return ++globalCounter;
}
