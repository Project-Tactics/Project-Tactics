#pragma once

#include <Libs/Fsm/FsmExternalController.h>

namespace tactics {

class DefaultFsmExternalController : public FsmExternalController {
public:
	FsmAction update(const HashId& currentStateName) override;
	void setNextTransition(const HashId& nextTransition);
	const HashId& getCurrentStateName() const;

private:
	FsmAction _nextTransition;
	HashId _currentStateName;
};

} // namespace tactics
