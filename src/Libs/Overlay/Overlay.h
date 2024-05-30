#pragma once

#include <Libs/Overlay/OverlayTypes.h>

namespace tactics {

class Overlay {
public:
	virtual ~Overlay() = default;
	virtual OverlayConfig getConfig() { return {}; }
	virtual void update() = 0;
};

}
