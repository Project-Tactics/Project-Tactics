#pragma once

#include <glm/glm.hpp>
#include <imgui.h>

namespace tactics {
namespace resource {
class Texture;
}

void* toImGuiTexture(const resource::Texture& texture);
ImVec2 toImVec2(const glm::vec2& vec);
bool vector3(const char* label,
			 glm::vec3& vec,
			 float componentWidth,
			 float componentSpeed,
			 const ImVec4& xColor,
			 const ImVec4& yColor,
			 const ImVec4& zColor);
bool vector2(const char* label,
			 glm::vec2& vec,
			 float componentWidth,
			 float componentSpeed,
			 const ImVec4& xColor,
			 const ImVec4& yColor);
bool vecComponent(float* component, const ImVec4& color, float width, float speed, const char* id);

} // namespace tactics
