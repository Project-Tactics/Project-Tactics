#pragma once

#include <vector>

namespace tactics {

class EventsListener;

enum class EventResult: bool {
	None,
	QuitGame
};

class EventsSystem {
public:
	EventResult update();
	void registerEventsListener(EventsListener* listener);
	void unregisterEventsListener(EventsListener* listener);

private:
	std::vector<EventsListener*> _eventsListeners;
};

}
