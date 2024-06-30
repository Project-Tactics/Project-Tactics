#pragma once

#include <chrono>

namespace tactics {

class FrameTimer {
public:
	void setFixedDeltaTime(double fixedDeltaTime) noexcept;
	[[nodiscard]] double getFixedDeltaTime() const noexcept;

	[[nodiscard]] bool hasConsumedAllTicks() const noexcept;
	void consumeTick() noexcept;

	void reset(double time);
	void update(double newTime);
	[[nodiscard]] unsigned int getTicks() const noexcept;

private:
	double _fixedDeltaTime{};
	double _timeAccumulation{};
	double _currentTime;
};

} // namespace tactics
