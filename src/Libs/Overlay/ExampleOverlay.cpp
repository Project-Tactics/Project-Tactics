#include "ExampleOverlay.h"

#include <sol/sol.hpp>
#include <imgui/imgui.h>

namespace tactics {

// TODO(Gerark) this is going to disappear asap
sol::state state;

ExampleOverlay::ExampleOverlay() {
	state.open_libraries(sol::lib::base);

	state.set_function("draw", [] () {
		ImGui::ShowDemoWindow();
	});
}

void ExampleOverlay::update() {
	state.script("draw()");
}

}
