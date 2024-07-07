#include "ImGuiUtilities.h"

#include <Libs/Resource/Texture/Texture.h>

namespace tactics {

void* toImGuiTexture(const resource::Texture& texture) {
	return (void*)(intptr_t)(texture.rendererId);
}

ImVec2 toImVec2(const glm::vec2& vec) {
	return ImVec2(vec.x, vec.y);
}

} // namespace tactics
