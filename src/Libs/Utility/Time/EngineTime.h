#pragma once

#include <Libs/Utility/Time/FrameTimer.h>

namespace tactics {

class EngineTime {
public:
	template<typename T> static [[nodiscard]] T fixedDeltaTime() noexcept { return static_cast<T>(_fixedDeltaTime()); }

	static void setFrameTime(FrameTimer* frameTimer) noexcept;

private:
	static [[nodiscard]] double _fixedDeltaTime() noexcept;
	static FrameTimer* _frameTimer;
};

} // namespace tactics
