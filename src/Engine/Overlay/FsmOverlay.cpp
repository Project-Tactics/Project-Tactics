#include "FsmOverlay.h"

#include "CustomOverlayColors.h"

#include <Engine/Core/DefaultFsmExternalController.h>

#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/Utility/String.h>
#include <Libs/Utility/Exception.h>

#include <imgui_node_editor.h>
#include <algorithm>
#include <ranges>

namespace tactics {

FsmOverlay::FsmOverlay(DefaultFsmExternalController& externalController, FsmInfo& fsmInfo)
	: _externalController(externalController)
	, _fsmInfo(fsmInfo) {
	_nodeGraphContext = ax::NodeEditor::CreateEditor();
}

OverlayConfig FsmOverlay::getConfig() {
	OverlayConfig config;
	config.position = {5, 30};
	config.size = {700, 400};
	config.isMenuBarButton = true;
	return config;
}

void FsmOverlay::update() {
	if (!_isAnyButtonTransitionHovered) {
		_hoveredStateTransition.clear();
	}
	_isAnyButtonTransitionHovered = false;

	ImGui::BeginGroup();
	_drawStateList();
	ImGui::Separator();
	_drawCurrentState();
	ImGui::EndGroup();
	ImGui::SameLine();
	_drawNodeGraph();
}

void FsmOverlay::_drawCurrentState() {
	auto& currentStateName = _externalController.getCurrentStateName();
	auto& state = _getStateInfo(currentStateName);
	auto& colors = CustomOverlayColors::getColors();

	ImGui::BeginGroup();

	ImGui::TextColored(colors.TitleTextColor, "TRANSITIONS");
	for (const auto& [transition, targets] : state.transitions) {
		_drawTransitionButton(transition, {100, 0});
	}
	ImGui::EndGroup();
}

void FsmOverlay::_drawStateList() {
	auto& colors = CustomOverlayColors::getColors();
	ImGui::BeginGroup();
	ImGui::TextColored(colors.TitleTextColor, "STATES");
	ImGui::Separator();
	for (auto& state : _fsmInfo.states) {
		ImGui::Text("%s", state.name.c_str());
		if (state.name == _fsmInfo.startState) {
			ImGui::SameLine();
			ImGui::TextColored({0.4f, 0.4f, 0.95f, 1.f}, "[START]");
		} else if (state.name == _externalController.getCurrentStateName()) {
			ImGui::SameLine();
			ImGui::TextColored({0, 0.75f, 0.5f, 1.f}, "[CURRENT]");
		}
	}
	ImGui::EndGroup();
}

void FsmOverlay::_drawNodeGraph() {
	auto& state = _getStateInfo(_externalController.getCurrentStateName());

	ax::NodeEditor::SetCurrentEditor(_nodeGraphContext);
	ax::NodeEditor::Begin("FSM", ImVec2(0.0, 0.0f));

	auto pos = ax::NodeEditor::ScreenToCanvas(ImVec2(100, 100));

	auto currentStatePos = ImVec2(50, 150);
	_drawNode(state, currentStatePos, false, true);
	ax::NodeEditor::SelectNode(hash(state.name).value());

	auto transitionOffsetY = currentStatePos.y - ((state.transitions.size() / 2) * 50);
	auto transitionOffsetX = currentStatePos.x + 200;
	auto transitionIndex = 0;
	for (auto& [transition, targets] : state.transitions) {
		auto& targetState = _getStateInfo(targets[0].stateName);
		auto targetStatePos = ImVec2(transitionOffsetX, transitionOffsetY + transitionIndex * 100);
		_drawNode(targetState, targetStatePos, true, false);
		ax::NodeEditor::SelectNode(hash(targetState.name).value(), true);
		++transitionIndex;
	}

	_drawLinks(state);

	ax::NodeEditor::NavigateToSelection(true, 0);
	ax::NodeEditor::ClearSelection();

	ax::NodeEditor::End();
	ax::NodeEditor::SetCurrentEditor(nullptr);
}

void FsmOverlay::_drawTransitionButton(const std::string& transition, const ImVec2& position) {
	auto& colors = CustomOverlayColors::getColors();

	if (_hoveredStateTransition == transition) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
	} else {
		ImGui::PushStyleColor(ImGuiCol_Button, colors.SelectedButtonColor);
	}

	if (ImGui::Button(transition.c_str(), position)) {
		_externalController.setNextTransition(transition);
	}

	if (ImGui::IsItemHovered()) {
		_hoveredStateTransition = transition;
		_isAnyButtonTransitionHovered |= true;
	}

	ImGui::PopStyleColor();
}

FsmStateInfo& FsmOverlay::_getStateInfo(const std::string& stateName) {
	auto itr = std::ranges::find_if(_fsmInfo.states, [&stateName] (const FsmStateInfo& stateInfo) {
		return stateInfo.name == stateName;
	});

	if (itr == _fsmInfo.states.end()) {
		throw TACTICS_EXCEPTION("State {} not found", stateName);
	}

	return *itr;
}

void FsmOverlay::_drawNodeTitleBar(FsmStateInfo& state) {
	ImGui::Text("%s", state.name.c_str());
}

void FsmOverlay::_drawNodeInputPorts(FsmStateInfo& state) {
	ImGui::BeginGroup();

	ax::NodeEditor::BeginPin(hash(state.name + "Enter").value(), ax::NodeEditor::PinKind::Input);
	ImGui::Text("-> Enter");
	ax::NodeEditor::EndPin();

	ImGui::EndGroup();
}

void FsmOverlay::_drawNodeOutputPorts(FsmStateInfo& state) {
	ImGui::BeginGroup();

	for (auto& [transition, targets] : state.transitions) {
		ax::NodeEditor::BeginPin(hash(state.name + transition).value(), ax::NodeEditor::PinKind::Output);
		_drawTransitionButton(transition, {100, 0});
		ax::NodeEditor::EndPin();
	}

	ImGui::EndGroup();
}

void FsmOverlay::_drawNode(FsmStateInfo& state, std::optional<ImVec2> position, bool showInput, bool showOutput) {
	auto stateId = hash(state.name).value();
	if (position.has_value()) {
		ax::NodeEditor::SetNodePosition(stateId, *position);
	}
	ax::NodeEditor::BeginNode(stateId);

	_drawNodeTitleBar(state);
	if (showInput) {
		_drawNodeInputPorts(state);
	}
	if (showInput && showOutput) {
		ImGui::SameLine();
	}
	if (showOutput) {
		_drawNodeOutputPorts(state);
	}

	ax::NodeEditor::EndNode();
}

void FsmOverlay::_drawLinks(FsmStateInfo& state) {
	auto& colors = CustomOverlayColors::getColors();
	for (auto& [transition, targets] : state.transitions) {
		auto& targetState = _getStateInfo(targets[0].stateName);
		ax::NodeEditor::Link(
			hash(state.name + targetState.name).value(),
			hash(state.name + transition).value(),
			hash(targetState.name + "Enter").value(),
			_hoveredStateTransition == transition ? colors.SelectedButtonColor : ImVec4{1, 1, 1, 1},
			_hoveredStateTransition == transition ? 3.0f : 1.0f
		);
	}
}

}
