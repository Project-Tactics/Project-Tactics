#pragma once

namespace tactics::component {

struct InputComponent {
	std::vector<click::ActionId> actions;
	std::vector<std::tuple<click::InputState, click::ActionValue>> values;
};

} // namespace tactics::component
