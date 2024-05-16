#include "ImGuiRenderSteps.h"

#include <Libs/Overlay/OverlaySystem.h>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_sdl2.h>

namespace tactics::renderstep {

void ImGuiBegin::render() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void ImGuiEnd::render() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

ImGuiRender::ImGuiRender(OverlaySystem& overlaySystem): _overlaySystem(overlaySystem) {
}

void ImGuiRender::render() {
	_overlaySystem.update();
}

}
