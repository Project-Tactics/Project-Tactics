#pragma once

#include <Libs/Overlay/Overlay.h>

#include <glm/glm.hpp>
#include <imgui/imgui.h>

namespace tactics {

class RenderSystem;
class OverlaySystem;

class DebugOverlay: public Overlay {
public:
	DebugOverlay(RenderSystem& renderSystem);
	void update() override;

private:
	bool _vector3(const char* label, glm::vec3& vec, float componentWidth = 0);
	bool _vecComponent(float* component, const ImVec4& color, float width, const char* id);

	RenderSystem& _renderSystem;
};

}
