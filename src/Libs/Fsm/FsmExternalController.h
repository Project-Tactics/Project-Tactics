#pragma once

#include "FsmTypes.h"

namespace tactics {
class FsmExternalController {
public:
	virtual ~FsmExternalController() = default;
	virtual FsmAction update(const HashId& currentStateName) = 0;
};

}
