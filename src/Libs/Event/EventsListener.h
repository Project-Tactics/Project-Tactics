#pragma once

#include <SDL_events.h>

namespace tactics {

/**
 * @brief Abstract class for managing system events.
 *
 * This abstract class can be implemented to handle various system events such as input,
 * window events, gamepad events, and more.
 */
class EventsListener {
public:
	virtual ~EventsListener() = default;
	virtual bool onEvent(SDL_Event& event) = 0;
};

} // namespace tactics
