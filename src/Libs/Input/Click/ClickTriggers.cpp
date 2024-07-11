#include "ClickTriggers.h"

namespace click {

bool _isOverActuationThreshold(float actuationThreshold, const ActionValue& value) {
	auto magnitudeSquared = value.vec3.x * value.vec3.x + value.vec3.y * value.vec3.y + value.vec3.z * value.vec3.z;
	return magnitudeSquared >= actuationThreshold * actuationThreshold;
}

} // namespace click

namespace click::down {

TriggerState update(Trigger& trigger, float /*deltaTime*/) {
	return trigger.state;
}

void processEvent(Trigger& trigger, const ActionValue& value) {
	if (_isOverActuationThreshold(trigger.data.down.actuationThreshold, value)) {
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
	if (_isOverActuationThreshold(trigger.data.press.actuationThreshold, value)) {
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
	if (_isOverActuationThreshold(trigger.data.down.actuationThreshold, value)) {
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

namespace click::continuous {

TriggerState update(Trigger& trigger, float /*deltaTime*/) {
	return trigger.state;
}

void processEvent(Trigger& trigger, const ActionValue& value) {
	if (_isOverActuationThreshold(trigger.data.continuous.actuationThreshold, value)) {
		trigger.state = TriggerState::Triggered;
	} else {
		trigger.state = TriggerState::Idle;
	}
}

} // namespace click::continuous
