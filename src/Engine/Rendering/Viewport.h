#pragma once

#include <glm/glm.hpp>

namespace tactics {

class Viewport {
public:
	Viewport(const glm::ivec2& size);
	void setSize(const glm::ivec2& size);
	int getWidth() const;
	int getHeight() const;
	const glm::ivec2& getSize() const;

private:
	glm::ivec2 _size;
};

}
