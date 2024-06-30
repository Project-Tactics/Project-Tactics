#include "ImGuiRenderSteps.h"

#include <Libs/Overlay/OverlaySystem.h>

#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_sdl2.h>
#include <imgui/imgui.h>

namespace tactics::renderstep {

ImGuiRender::ImGuiRender(OverlaySystem& overlaySystem) : _overlaySystem(overlaySystem) {}

void ImGuiRender::execute(RenderStepInfo&) {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	_overlaySystem.update();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // namespace tactics::renderstep
