#pragma once

#include <Libs/Fsm/FsmExternalController.h>

namespace tactics {

class DefaultFsmExternalController: public FsmExternalController {
public:
	FsmAction update(std::string_view currentStateName) override;
	void setNextTransition(std::string nextTransition);
	const std::string& getCurrentStateName() const;

private:
	FsmAction _nextTransition;
	std::string _currentStateName;
};

}
