#pragma once

#include "ClickInputTypes.h"

namespace click {

[[nodiscard]] TriggerState update(DownTrigger& trigger, Binding& binding, float deltaTime);
[[nodiscard]] TriggerState update(PressTrigger& trigger, Binding& binding, float deltaTime);
[[nodiscard]] TriggerState update(ReleaseTrigger& trigger, Binding& binding, float deltaTime);
[[nodiscard]] TriggerState update(HoldTrigger& trigger, Binding& binding, float deltaTime);
[[nodiscard]] TriggerState update(ContinuousTrigger& trigger, Binding& binding, float deltaTime);

} // namespace click

