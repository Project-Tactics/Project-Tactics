#include "DrawSomethingRenderStep.h"

#include <glad/gl.h>

namespace tactics::renderstep {

void DrawSomething::render() {
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

}
