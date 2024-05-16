#pragma once

#include "RenderStep.h"

namespace tactics {
class OverlaySystem;
}

namespace tactics::renderstep {

class ImGuiBegin: public RenderStep {
public:
	void render() override;
};

class ImGuiEnd: public RenderStep {
public:
	void render() override;
};

class ImGuiRender: public RenderStep {
public:
	ImGuiRender(OverlaySystem& overlaySystem);
	void render() override;

private:
	OverlaySystem& _overlaySystem;
};

}
