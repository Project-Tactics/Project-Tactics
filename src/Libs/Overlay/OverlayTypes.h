#pragma once

#include <imgui.h>

namespace tactics {

template<typename TOverlay, typename U>
concept HasType = requires(TOverlay t) {
	{ t.U::TYPE };
};

class Overlay;

enum class OverlayType {
	Window,
	MenuBar,
	Custom
};

struct OverlayConfig {
	ImVec2 position{};
	ImVec2 size{};
	bool isMenuBarButton{};
};

} // namespace tactics
