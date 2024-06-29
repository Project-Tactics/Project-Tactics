#pragma once

#include "FsmState.h"

#include <Libs/Utility/HashId.h>

#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>

namespace tactics {

struct FsmTransitionTarget {
	HashId stateName;
	std::function<bool()> condition{};
};

using FsmTransitions = std::unordered_map<HashId, std::vector<FsmTransitionTarget>>;

struct FsmStateEntry {
	HashId name;
	std::unique_ptr<FsmState> state;
	FsmTransitions transitions;
};

using FsmStateEntries = std::unordered_map<HashId, std::unique_ptr<FsmStateEntry>>;

struct FsmStateInfo {
	HashId name;
	FsmTransitions transitions;
};

struct FsmInfo {
	std::vector<FsmStateInfo> states;
	HashId startState;
};

}
