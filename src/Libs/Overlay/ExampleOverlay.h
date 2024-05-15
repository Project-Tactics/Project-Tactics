#pragma once

#include "Overlay.h"

namespace tactics {

class ExampleOverlay: public Overlay {
public:
	ExampleOverlay();
	virtual void update() override;
};

}
