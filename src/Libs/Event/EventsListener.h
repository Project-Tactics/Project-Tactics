#pragma once

union SDL_Event;

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
	 * Listeners are notified in registration order.
	 *
	 * @param event The SDL event to process.
	 * @return true if the event has been consumed and must not be forwarded to the remaining listeners.
	 */
	[[nodiscard]] virtual bool onEvent(const SDL_Event& event) = 0;
};

} // namespace tactics
