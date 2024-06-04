#pragma once

#include <Libs/Overlay/Overlay.h>

#include <string>
#include <optional>

namespace ax::NodeEditor {
struct EditorContext;
}

namespace tactics {

class DefaultFsmExternalController;
struct FsmInfo;
struct FsmStateInfo;

class FsmOverlay: public Overlay {
public:
	FsmOverlay(DefaultFsmExternalController& externalController, FsmInfo& fsmInfo);
	void update() override;
	OverlayConfig getConfig() override;

private:
	void _drawCurrentState();
	void _drawNodeGraph();
	void _drawNode(FsmStateInfo& state, std::optional<ImVec2> position, bool showInput, bool showOutput);
	void _drawNodeTitleBar(FsmStateInfo& state);
	void _drawNodeInputPorts(FsmStateInfo& state);
	void _drawNodeOutputPorts(FsmStateInfo& state);
	void _drawLinks(FsmStateInfo& state);
	void _drawTransitionButton(const std::string& transition, const ImVec2& pos);
	void _drawStateList();

	[[nodiscard]] FsmStateInfo& _getStateInfo(const std::string& stateName);

	DefaultFsmExternalController& _externalController;
	FsmInfo& _fsmInfo;
	std::string _hoveredStateTransition;
	bool _isAnyButtonTransitionHovered{};
	ax::NodeEditor::EditorContext* _nodeGraphContext;
};

}
