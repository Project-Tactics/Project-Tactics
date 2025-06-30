#pragma once

#include <Engine/Core/Application.h>

namespace tactics {

/*
 * A Prototype for OpenTactica
 * The idea is to implement a simple version of the final application
 * and using this as a playground to identify required features that
 * should be implemented in the engine
 *
 * The Prototype is composed by:
 * - Main Menu - where the player can select the map to play on
 * - Options - to adjust the most common settings ( Fullscreen, Resolution, Volume, etc. )
 * - Game
 *		- Map/Grid Movement
 *		- Turn Management
 *		- Spell System
 *		- Character Management
 *		- Win/Lose Conditions
 */
class OpenTacticaPrototypeApplication : public tactics::Application {
public:
	void setupComponentReflections() override;
	HashId initialize(ServiceLocator& serviceLocator, FsmBuilder& fsmBuilder) override;
};

} // namespace tactics

