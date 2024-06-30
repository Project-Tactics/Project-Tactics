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

	/**
	 * @brief Main function that handles dispatching events to the appropriate protected virtual methods.
	 *
	 * Implementers can override only the methods they are interested in, without needing to determine
	 * the event type from SDL.
	 *
	 * @param event The SDL event to be processed.
	 * @return Returns true to indicate that the event has been processed, so the system does not need to handle it
	 * further.
	 */
	virtual bool onEvent(SDL_Event& event);

protected:
	/**
	 * @brief Handles keyboard press events.
	 *
	 * This function processes SDL keyboard events. Implementers can override this method to handle
	 * specific key press events.
	 *
	 * @param event The SDL keyboard event to be processed.
	 * @return Returns true to indicate that the key press event has been handled.
	 */
	virtual bool onKeyPress(SDL_KeyboardEvent& event);

	/**
	 * @brief Handles keyboard release events.
	 *
	 * This function processes SDL keyboard events. Implementers can override this method to handle
	 * specific key release events.
	 *
	 * @param event The SDL keyboard event to be processed.
	 * @return Returns true to indicate that the key press event has been handled.
	 */
	virtual bool onKeyRelease(SDL_KeyboardEvent& event);
};

} // namespace tactics
