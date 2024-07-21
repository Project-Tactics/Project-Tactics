#pragma once

#include "ClickInputTypes.h"

namespace click {

[[nodiscard]] ConditionState update(DownCondition& condition, Binding& binding, float deltaTime);
[[nodiscard]] ConditionState update(PressCondition& condition, Binding& binding, float deltaTime);
[[nodiscard]] ConditionState update(ReleaseCondition& condition, Binding& binding, float deltaTime);
[[nodiscard]] ConditionState update(HoldCondition& condition, Binding& binding, float deltaTime);
[[nodiscard]] ConditionState update(ContinuousCondition& condition, Binding& binding, float deltaTime);

} // namespace click

