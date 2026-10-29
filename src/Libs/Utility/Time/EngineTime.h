#pragma once

#include <Libs/Utility/Time/FrameTimer.h>

namespace tactics {

class EngineTime {
public:
	template<typename T> [[nodiscard]] static T fixedDeltaTime() noexcept {
		return static_cast<T>(_fixedDeltaTime());
	}

	static void setFrameTime(FrameTimer* frameTimer) noexcept;

private:
	[[nodiscard]] static double _fixedDeltaTime() noexcept;
	static FrameTimer* _frameTimer;
};

} // namespace tactics
