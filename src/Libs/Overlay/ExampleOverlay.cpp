#include "ExampleOverlay.h"

#include <sol/sol.hpp>
#include <imgui/imgui.h>

namespace tactics {

// TODO(Gerark) this is going to disappear asap. We keep it here just to compile/link sol2/lua even if we're not going to use scripting immediately
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
