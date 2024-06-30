#pragma once

#include "OverlayTypes.h"

#include <Libs/Utility/TransparentTypes.h>

#include <functional>
#include <memory>

namespace tactics {
namespace resource {
class IniFile;
}
class FileSystem;

/**
 * @brief An Overlay is a screen built in Immediate Mode GUI ( imgui ) which is displayed on top of any other ui and
 * it's mainly used for debug purposes ( Console, Render Stats, Debug View Buttons, etc... ).
 *
 * The Overlay System is the class responsible to keep track of all the registered Overlays and update them accordingly.
 */
class OverlaySystem {
public:
	OverlaySystem(std::shared_ptr<resource::IniFile> devUserSettings,
				  resource::IniFile& imGuiSettings,
				  const FileSystem& fileSystem);
	~OverlaySystem();

	template<typename TOverlay, typename... TArgs>
	void addOverlay(std::string_view name, bool enabled, TArgs&&... args) {
		auto overlay = std::make_unique<TOverlay>(std::forward<TArgs>(args)...);
		if constexpr (HasType<TOverlay, TOverlay>) {
			_addOverlay(name, std::move(overlay), TOverlay::TYPE, enabled);
		} else {
			_addOverlay(name, std::move(overlay), OverlayType::Window, enabled);
		}
	}

	void removeOverlay(std::string_view name);
	void enableOverlay(std::string_view name, bool enabled);

	bool isEnabled() const;
	void setEnabled(bool enable);

	void update();

	struct OverlayItem {
		std::unique_ptr<Overlay> overlay;
		OverlayConfig config{};
		OverlayType type{};
		bool enabled{};
	};

	void forEachOverlay(const std::function<void(const std::string&, OverlayItem&)>& callback);

private:
	void _addOverlay(std::string_view name,
					 std::unique_ptr<Overlay> overlay,
					 OverlayType type = OverlayType::Window,
					 bool enabled = false);
	bool _getOrCreateOverlayStoredEnableValue(std::string_view name, bool defaultValue);
	void _setOverlayStoredEnableValue(std::string_view name, bool enabled);

	UnorderedStringMap<OverlayItem> _overlays;
	bool _isEnabled{};
	std::shared_ptr<resource::IniFile> _devUserSettings;
};

} // namespace tactics
