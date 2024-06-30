#pragma once

#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/Overlay/Overlay.h>

#include <optional>
#include <string>

namespace ax::NodeEditor {
struct EditorContext;
}

namespace tactics {

class DefaultFsmExternalController;
struct FsmInfo;
struct FsmStateInfo;
struct FsmTransitionTarget;

class FsmOverlay : public Overlay {
public:
	FsmOverlay(DefaultFsmExternalController& externalController, FsmInfo& fsmInfo);
	void update() override;
	OverlayConfig getConfig() override;

private:
	void _drawSidePanel();
	void _drawNodeGraph();

	[[nodiscard]] FsmStateInfo& _getStateInfo(const HashId& stateName);

	DefaultFsmExternalController& _externalController;
	FsmInfo& _fsmInfo;

	struct Link {
		HashId id;
		HashId outputPinId;
		HashId inputPinId;
		bool highlighted{};
	};

	struct Pin {
		HashId id;
		HashId name;
		bool highlighted{};
	};

	struct Target {
		HashId id;
		HashId name;
		Pin inputPin;
	};

	struct Model {
		HashId currentStateName;
		HashId currentStateId;
		std::vector<Pin> outputPins;
		std::vector<Link> links;
		std::vector<Target> targets;
		bool isAnyHovered;
	};

	void _buildModel();
	void _drawMainState();
	void _drawTargetStates();
	void _drawTargetState(Target& target, const ImVec2& position);
	void _drawTargetStatePorts(Target& target);
	void _drawMainStatePorts();
	void _drawTransitionButton(Pin& pin, const ImVec2& position);
	void _drawLinks();

	Model _model;
	ax::NodeEditor::EditorContext* _nodeGraphContext;
};

} // namespace tactics
