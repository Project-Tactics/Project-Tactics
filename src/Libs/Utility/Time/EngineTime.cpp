#include "EngineTime.h"

namespace tactics {

FrameTimer* EngineTime::_frameTimer = nullptr;

void EngineTime::setFrameTime(FrameTimer* frameTimer) noexcept { _frameTimer = frameTimer; }

[[nodiscard]] double EngineTime::_fixedDeltaTime() noexcept { return _frameTimer->getFixedDeltaTime(); }

} // namespace tactics
