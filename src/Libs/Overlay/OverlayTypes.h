#pragma once

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

}
