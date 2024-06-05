#pragma once

#include <Libs/Overlay/Overlay.h>
#include <Libs/Ecs/EntityUtilities.h>

#include <string>
#include <optional>

namespace ax::NodeEditor {
struct EditorContext;
}

namespace tactics {

class DefaultFsmExternalController;
struct FsmInfo;
struct FsmStateInfo;
struct FsmTransitionTarget;

class FsmOverlay: public Overlay {
public:
	FsmOverlay(DefaultFsmExternalController& externalController, FsmInfo& fsmInfo);
	void update() override;
	OverlayConfig getConfig() override;

private:
	void _drawSidePanel();
	void _drawNodeGraph();

	[[nodiscard]] FsmStateInfo& _getStateInfo(const std::string& stateName);

	DefaultFsmExternalController& _externalController;
	FsmInfo& _fsmInfo;

	struct Link {
		hash_string id;
		hash_string outputPinId;
		hash_string inputPinId;
		bool highlighted{};
	};

	struct Pin {
		hash_string id;
		std::string name;
		bool highlighted{};
	};

	struct Target {
		hash_string id;
		std::string name;
		Pin inputPin;
	};

	struct Model {
		std::string currentStateName;
		hash_string currentStateId;
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

}
