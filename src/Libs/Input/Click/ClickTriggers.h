#pragma once

#include "ClickInputTypes.h"

namespace click::down {

[[nodiscard]] TriggerState update(Trigger& trigger, float deltaTime);
void processEvent(Trigger& trigger, const ActionValue& value);

} // namespace click::down

namespace click::press {

[[nodiscard]] TriggerState update(Trigger& trigger, float deltaTime);
void processEvent(Trigger& trigger, const ActionValue& value);

} // namespace click::press

namespace click::release {

[[nodiscard]] TriggerState update(Trigger& trigger, float deltaTime);
void processEvent(Trigger& trigger, const ActionValue& value);

} // namespace click::release

namespace click::hold {

[[nodiscard]] TriggerState update(Trigger& trigger, float deltaTime);
void processEvent(Trigger& trigger, const ActionValue& value);

} // namespace click::hold

namespace click::continuous {

[[nodiscard]] TriggerState update(Trigger& trigger, float deltaTime);
void processEvent(Trigger& trigger, const ActionValue& value);

} // namespace click::continuous

