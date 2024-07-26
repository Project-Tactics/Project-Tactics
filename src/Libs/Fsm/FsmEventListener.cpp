#include "FsmEventListener.h"

namespace tactics {

const HashId FsmEventListener::appExitRequestTransition = "_appExitRequest"_id;

FsmEventAction FsmEventListener::onEvent(SDL_Event& event) {
	switch (event.type) {
	case SDL_QUIT: {
		return FsmEventAction::transition(appExitRequestTransition);
	}
	}
	return FsmEventAction::none();
}

} // namespace tactics
