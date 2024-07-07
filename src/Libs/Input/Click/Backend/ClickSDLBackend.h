#pragma once

#include <SDL.h>

namespace click {

void initSdlBackend();
void shutdownSdlBackend();
void processSdlEvents(SDL_Event& event);

} // namespace click
