#pragma once

#include <vector>

namespace tactics {

class EventsListener;
class InputSystem;

class EventsSystem {
public:
	EventsSystem(InputSystem& inputSystem);
	void update();
	void registerEventsListener(EventsListener* listener);
	void unregisterEventsListener(EventsListener* listener);

private:
	std::vector<EventsListener*> _eventsListeners;
	InputSystem& _inputSystem;
};

} // namespace tactics
