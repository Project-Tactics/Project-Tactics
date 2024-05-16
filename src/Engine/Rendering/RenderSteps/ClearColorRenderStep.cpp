#include "ClearColorRenderStep.h"

#include <glad/gl.h>

namespace tactics {

void ClearColorRenderStep::render() {
	glClear(GL_COLOR_BUFFER_BIT);
}

}
