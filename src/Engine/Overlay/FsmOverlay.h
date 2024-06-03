#pragma once

#include <Libs/Overlay/Overlay.h>

namespace tactics {

class Fsm;

class FsmOverlay: public Overlay {
public:
	FsmOverlay();
	void update() override;

private:
	Fsm& _fsm;
};

}
