#pragma once

#include <Libs/Event/EventsListener.h>
#include <Libs/Fsm/FsmExternalController.h>

namespace tactics {

class DefaultFsmExternalController : public FsmExternalController, public EventsListener {
public:
	FsmAction update(const HashId& currentStateName) override;
	void setNextTransition(const HashId& nextTransition);
	const HashId& getCurrentStateName() const;

	bool onEvent(SDL_Event& event) override;

private:
	FsmAction _nextTransition;
	HashId _currentStateName;
};

} // namespace tactics
