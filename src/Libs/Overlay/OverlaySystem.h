#pragma once

#include <Libs/Utilities/TransparentTypes.h>

#include <memory>

namespace tactics {

class Overlay;

/**
 * @brief An Overlay is a screen built in Immediate Mode GUI ( imgui ) which is displayed on top of any other ui and it's
 * mainly used for debug purposes ( Console, Render Stats, Debug View Buttons, etc... ).
 *
 * The Overlay System is the class responsible to keep track of all the registered Overlays and update them accordingly.
 */
class OverlaySystem {
public:
	OverlaySystem();
	~OverlaySystem();

	void addOverlay(std::string_view name, std::unique_ptr<Overlay> overlay);
	void removeOverlay(std::string_view name);

	bool isEnabled() const;
	void setEnabled(bool enable);

	void drawOverlays();

private:
	void _update();

	UnorderedStringMap<std::unique_ptr<Overlay>> _overlays;
	bool _isEnabled{};
};

}
