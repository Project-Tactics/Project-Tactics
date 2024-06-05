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

const ImVec2 currentStatePosition = {50, 150};

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
	_buildModel();

	_drawSidePanel();
	ImGui::SameLine();
	_drawNodeGraph();
}

void FsmOverlay::_buildModel() {
	hash_string highlightedOutputPinId;
	if (_model.isAnyHovered) {
		auto itr = std::ranges::find_if(_model.outputPins, [] (auto& pin) {
			return pin.highlighted;
		});
		if (itr != _model.outputPins.end()) {
			highlightedOutputPinId = itr->id;
		}
	}

	_model.links.clear();
	_model.outputPins.clear();
	_model.targets.clear();
	_model.isAnyHovered = false;

	auto& currentState = _getStateInfo(_externalController.getCurrentStateName());
	_model.currentStateName = currentState.name;
	_model.currentStateId = hash(currentState.name);
	for (auto& [transition, targets] : currentState.transitions) {
		auto outputPinId = hash(transition);
		_model.outputPins.emplace_back(outputPinId, transition, outputPinId == highlightedOutputPinId);
		for (auto& target : targets) {
			auto inputPinId = hash(target.stateName + "enter");
			auto highlighted = outputPinId == highlightedOutputPinId;
			_model.links.emplace_back(
				hash(currentState.name + transition + target.stateName),
				outputPinId,
				inputPinId,
				highlighted
			);

			auto targetId = hash(target.stateName);
			auto alreadyExists = std::ranges::any_of(_model.targets, [&targetId] (const auto& target) {
				return target.id == targetId;
			});
			if (!alreadyExists) {
				_model.targets.emplace_back(targetId, target.stateName, Pin{inputPinId, "enter", highlighted});
			}
		}
	}
}

void FsmOverlay::_drawSidePanel() {
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

	ImGui::Separator();

	ImGui::TextColored(colors.TitleTextColor, "TRANSITIONS");
	for (auto& pin : _model.outputPins) {
		_drawTransitionButton(pin, {100, 0});
	}

	ImGui::EndGroup();
}

void FsmOverlay::_drawNodeGraph() {
	ax::NodeEditor::SetCurrentEditor(_nodeGraphContext);
	ax::NodeEditor::Begin("FSM", ImVec2(0.0, 0.0f));

	_drawMainState();
	_drawTargetStates();
	_drawLinks();

	ax::NodeEditor::NavigateToSelection(true, 0);
	ax::NodeEditor::ClearSelection();

	ax::NodeEditor::End();
	ax::NodeEditor::SetCurrentEditor(nullptr);
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

void FsmOverlay::_drawMainState() {
	auto id = _model.currentStateId.value();
	ax::NodeEditor::SetNodePosition(id, currentStatePosition);
	ax::NodeEditor::BeginNode(id);
	ImGui::Text("%s", _model.currentStateName.c_str());
	_drawMainStatePorts();
	ax::NodeEditor::EndNode();
	ax::NodeEditor::SelectNode(id);
}

void FsmOverlay::_drawMainStatePorts() {
	ImGui::BeginGroup();

	for (auto& pin : _model.outputPins) {
		ax::NodeEditor::BeginPin(pin.id.value(), ax::NodeEditor::PinKind::Output);
		_drawTransitionButton(pin, {100, 0});
		ImGui::SameLine();
		auto radius = 4.0f;
		auto pos = ImGui::GetCursorPos();
		auto center = ImVec2(pos.x - 4, pos.y + radius + 7);
		ax::NodeEditor::PinPivotRect(center, center);
		auto drawList = ImGui::GetWindowDrawList();
		drawList->AddCircleFilled(center, radius, ImGui::ColorConvertFloat4ToU32({1, 1, 1, 1}));

		if (pin.highlighted) {
			auto& colors = CustomOverlayColors::getColors();
			drawList->AddCircle(
				center,
				radius + 1,
				ImGui::ColorConvertFloat4ToU32(colors.SelectedButtonColor), 0, 4);
		}

		ax::NodeEditor::EndPin();
	}

	ImGui::EndGroup();
}

void FsmOverlay::_drawTargetStates() {
	auto position = ImVec2{
		currentStatePosition.x + 200,
		currentStatePosition.y - ((_model.targets.size() / 2.f) * 50),
	};

	for (auto&& target : _model.targets) {
		_drawTargetState(target, position);
		ax::NodeEditor::SelectNode(target.id.value(), true);
		position.y += 100;
	}
}

void FsmOverlay::_drawTargetState(Target& target, const ImVec2& position) {
	ax::NodeEditor::SetNodePosition(target.id.value(), position);
	ax::NodeEditor::BeginNode(target.id.value());
	ImGui::Text("%s", target.name.c_str());
	_drawTargetStatePorts(target);
	ax::NodeEditor::EndNode();
}

void FsmOverlay::_drawTargetStatePorts(Target& target) {
	ImGui::BeginGroup();

	auto id = target.inputPin.id;
	ax::NodeEditor::BeginPin(id.value(), ax::NodeEditor::PinKind::Input);
	auto drawList = ImGui::GetWindowDrawList();

	auto radius = 4.0f;
	auto pos = ImGui::GetCursorPos();
	auto center = ImVec2(pos.x - 6, pos.y + radius);

	drawList->AddCircleFilled(center, radius, ImGui::ColorConvertFloat4ToU32({1, 1, 1, 1}));

	if (target.inputPin.highlighted) {
		auto& colors = CustomOverlayColors::getColors();
		drawList->AddCircle(
			center,
			radius + 1,
			ImGui::ColorConvertFloat4ToU32(colors.SelectedButtonColor), 0, 4);
	}

	ImGui::Dummy({-radius, radius * 2});
	ax::NodeEditor::EndPin();

	ImGui::EndGroup();
}

void FsmOverlay::_drawLinks() {
	auto& colors = CustomOverlayColors::getColors();

	for (auto& link : _model.links) {
		auto linkId = link.id.value();
		auto outputPinId = link.outputPinId.value();
		auto inputPinId = link.inputPinId.value();

		ax::NodeEditor::Link(
			linkId,
			outputPinId,
			inputPinId,
			link.highlighted ? colors.SelectedButtonColor : ImVec4{1, 1, 1, 1},
			link.highlighted ? 3.0f : 1.0f
		);
	}
}

void FsmOverlay::_drawTransitionButton(Pin& pin, const ImVec2& position) {
	auto& colors = CustomOverlayColors::getColors();

	if (pin.highlighted) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered]);
	} else {
		ImGui::PushStyleColor(ImGuiCol_Button, colors.SelectedButtonColor);
	}

	if (ImGui::Button(pin.name.c_str(), position)) {
		_externalController.setNextTransition(pin.name);
	}

	if (ImGui::IsItemHovered()) {
		pin.highlighted = true;
		_model.isAnyHovered |= true;
	}

	ImGui::PopStyleColor();
}

}
