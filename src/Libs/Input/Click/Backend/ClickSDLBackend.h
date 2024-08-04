#pragma once

union SDL_Event;

namespace click {

void initSdlBackend();
void shutdownSdlBackend();
void processSdlEvents(const SDL_Event& event);
void updateSdlBackend();

} // namespace click
