#include "ClickTriggers.h"

namespace click::down {

TriggerState update(Trigger& trigger, float /*deltaTime*/) {
	return trigger.state;
}

void processEvent(Trigger& trigger, const ActionValue& value) {
	if (std::abs(value.axis1D) >= trigger.data.down.actuationThreshold) {
		trigger.state = TriggerState::Triggered;
	} else {
		trigger.state = TriggerState::Idle;
	}
}

} // namespace click::down

namespace click::press {

TriggerState update(Trigger& trigger, float /*deltaTime*/) {
	if (trigger.state == TriggerState::Triggered) {
		return std::exchange(trigger.state, TriggerState::Idle);
	}
	return trigger.state;
}

void processEvent(Trigger& trigger, const ActionValue& value) {
	if (std::abs(value.axis1D) >= trigger.data.down.actuationThreshold) {
		trigger.state = TriggerState::Triggered;
	} else {
		trigger.state = TriggerState::Idle;
	}
}
} // namespace click::press

namespace click::release {

TriggerState update(Trigger& trigger, float /*deltaTime*/) {
	if (trigger.state == TriggerState::Triggered) {
		return std::exchange(trigger.state, TriggerState::Idle);
	}
	return trigger.state;
}

void processEvent(Trigger& trigger, const ActionValue& value) {
	if (std::abs(value.axis1D) < trigger.data.down.actuationThreshold) {
		trigger.state = TriggerState::Triggered;
	} else {
		trigger.state = TriggerState::Idle;
	}
}

} // namespace click::release

namespace click::hold {

TriggerState update(Trigger& trigger, float /*deltaTime*/) {
	return trigger.state;
}

void processEvent(Trigger& /*trigger*/, const ActionValue& /*value*/) {}

} // namespace click::hold
