#pragma once

#include <glm/glm.hpp>
#include <imgui.h>

namespace tactics {
namespace resource {
class Texture;
}

void* toImGuiTexture(const resource::Texture& texture);
ImVec2 toImVec2(const glm::vec2& vec);

} // namespace tactics
