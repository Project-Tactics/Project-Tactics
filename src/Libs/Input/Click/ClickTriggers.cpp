#include "ClickTriggers.h"

namespace click {

bool _isOverActuationThreshold(float actuationThreshold, const ActionValue& value) {
	return _magnitudeSquared(value) >= actuationThreshold * actuationThreshold;
}

TriggerState update(DownTrigger& trigger, Binding& binding, float /*deltaTime*/) {
	if (_isOverActuationThreshold(trigger.actuationThreshold, binding.value)) {
		return TriggerState::Triggered;
	}

	return TriggerState::Idle;
}

TriggerState update(PressTrigger& trigger, Binding& binding, float /*deltaTime*/) {
	if (_isOverActuationThreshold(trigger.actuationThreshold, binding.value)) {
		if (trigger.state == TriggerState::Idle) {
			trigger.state = TriggerState::Triggered;
			return TriggerState::Triggered;
		}
	}

	trigger.state = TriggerState::Idle;
	return TriggerState::Idle;
}

TriggerState update(ReleaseTrigger& trigger, Binding& binding, float /*deltaTime*/) {
	auto isOverThreshold = _isOverActuationThreshold(trigger.actuationThreshold, binding.value);
	switch (trigger.state) {
	case TriggerState::Idle: {
		if (isOverThreshold) {
			trigger.state = TriggerState::Ongoing;
			return trigger.state;
		}
		break;
	}
	case TriggerState::Ongoing: {
		if (!isOverThreshold) {
			trigger.state = TriggerState::Idle;
			return TriggerState::Triggered;
		}
		break;
	}
	}

	return trigger.state;
}

TriggerState update(HoldTrigger& trigger, Binding& binding, float deltaTime) {
	if (_isOverActuationThreshold(trigger.actuationThreshold, binding.value)) {
		trigger.state = TriggerState::Ongoing;
		trigger.currentTime += deltaTime;
		if (trigger.currentTime >= trigger.holdTime) {
			trigger.state = TriggerState::Triggered;
		}
	} else {
		trigger.state = TriggerState::Idle;
		trigger.currentTime = 0.0f;
	}

	return trigger.state;
}

TriggerState update(ContinuousTrigger& trigger, Binding& binding, float /*deltaTime*/) {
	if (_isOverActuationThreshold(trigger.actuationThreshold, binding.value)) {
		return TriggerState::Triggered;
	}

	return TriggerState::Idle;
}

} // namespace click
