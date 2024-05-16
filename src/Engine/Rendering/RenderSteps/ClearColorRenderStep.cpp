#include "ClearColorRenderStep.h"

#include <glad/gl.h>

namespace tactics::renderstep {

void ClearColor::render() {
	glClear(GL_COLOR_BUFFER_BIT);
}

}
