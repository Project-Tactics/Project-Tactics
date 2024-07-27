#include "ClickConditions.h"

namespace click {

bool _isOverActuationThreshold(float actuationThreshold, const ActionValue& value) {
	return _magnitudeSquared(value) >= actuationThreshold * actuationThreshold;
}

ConditionState update(DownCondition& condition, Binding& binding, float /*deltaTime*/) {
	if (_isOverActuationThreshold(condition.actuationThreshold, binding.value)) {
		return ConditionState::Triggered;
	}

	return ConditionState::Idle;
}

ConditionState update(PressCondition& condition, Binding& binding, float /*deltaTime*/) {
	switch (condition.state) {
	case ConditionState::Idle: {
		if (!_isOverActuationThreshold(condition.actuationThreshold, binding.value)) {
			condition.state = ConditionState::Ongoing;
		}
		break;
	}
	case ConditionState::Ongoing: {
		if (_isOverActuationThreshold(condition.actuationThreshold, binding.value)) {
			condition.state = ConditionState::Triggered;
		}
		break;
	}
	case ConditionState::Triggered: {
		condition.state = ConditionState::Idle;
	}
	}

	return condition.state;
}

ConditionState update(ReleaseCondition& condition, Binding& binding, float /*deltaTime*/) {
	auto isOverThreshold = _isOverActuationThreshold(condition.actuationThreshold, binding.value);
	switch (condition.state) {
	case ConditionState::Idle: {
		if (isOverThreshold) {
			condition.state = ConditionState::Ongoing;
			return condition.state;
		}
		break;
	}
	case ConditionState::Ongoing: {
		if (!isOverThreshold) {
			condition.state = ConditionState::Idle;
			return ConditionState::Triggered;
		}
		break;
	}
	}

	return condition.state;
}

ConditionState update(HoldCondition& condition, Binding& binding, float deltaTime) {
	if (_isOverActuationThreshold(condition.actuationThreshold, binding.value)) {
		condition.state = ConditionState::Ongoing;
		condition.currentTime += deltaTime;
		if (condition.currentTime >= condition.holdTime) {
			condition.state = ConditionState::Triggered;
		}
	} else {
		condition.state = ConditionState::Idle;
		condition.currentTime = 0.0f;
	}

	return condition.state;
}

ConditionState update(ContinuousCondition& condition, Binding& binding, float /*deltaTime*/) {
	if (_isOverActuationThreshold(condition.actuationThreshold, binding.value)) {
		return ConditionState::Triggered;
	}

	return ConditionState::Idle;
}

} // namespace click
