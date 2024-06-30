#pragma once

#include <vector>

namespace tactics {

class EventsListener;

class EventsSystem {
public:
	void update();
	void registerEventsListener(EventsListener* listener);
	void unregisterEventsListener(EventsListener* listener);

private:
	std::vector<EventsListener*> _eventsListeners;
};

} // namespace tactics
