#pragma once

#include "FsmTypes.h"

#include <string_view>

namespace tactics {
class FsmExternalController {
public:
	virtual ~FsmExternalController() = default;
	virtual FsmAction update(std::string_view currentStateName) = 0;
};

}
