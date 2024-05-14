#pragma once

#include <Libs/Utilities/TransparentTypes.h>
#include "FsmState.h"

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

namespace tactics {

struct FsmTransitionTarget {
	std::string stateName;
	std::function<bool()> condition{};
};

using FsmTransitions = UnorderedStringMap<std::vector<FsmTransitionTarget>>;

struct FsmStateEntry {
	std::string name;
	std::unique_ptr<FsmState> state;
	FsmTransitions transitions;
};

using FsmStateEntries = UnorderedStringMap<std::unique_ptr<FsmStateEntry>>;

}
