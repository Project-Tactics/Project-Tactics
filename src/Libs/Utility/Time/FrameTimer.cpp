#include "FrameTimer.h"

namespace tactics {

void FrameTimer::setFixedDeltaTime(double fixedDeltaTime) noexcept { _fixedDeltaTime = fixedDeltaTime; }

[[nodiscard]] double FrameTimer::getFixedDeltaTime() const noexcept { return _fixedDeltaTime; }

void FrameTimer::reset(double time) {
	_currentTime = time;
	_timeAccumulation = 0;
}

void FrameTimer::update(double newTime) {
	auto frameTime = newTime - _currentTime;
	_timeAccumulation += frameTime;
	_currentTime = newTime;
}

[[nodiscard]] unsigned int FrameTimer::getTicks() const noexcept {
	return static_cast<unsigned int>(_timeAccumulation / _fixedDeltaTime);
}

bool FrameTimer::hasConsumedAllTicks() const noexcept { return _timeAccumulation >= _fixedDeltaTime; }

void FrameTimer::consumeTick() noexcept { _timeAccumulation -= _fixedDeltaTime; }

} // namespace tactics
