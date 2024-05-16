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

	template<typename TOverlay, typename ...TArgs>
	void addOverlay(std::string_view name, TArgs... args) {
		auto overlay = std::make_unique<TOverlay>(std::forward(args)...);
		_addOverlay(name, std::move(overlay));
	}

	void removeOverlay(std::string_view name);

	bool isEnabled() const;
	void setEnabled(bool enable);

	void update();

private:
	void _addOverlay(std::string_view name, std::unique_ptr<Overlay> overlay);

	UnorderedStringMap<std::unique_ptr<Overlay>> _overlays;
	bool _isEnabled{};
};

}
