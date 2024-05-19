#include "DrawSomethingRenderStep.h"

#include <glad/gl.h>

namespace tactics::renderstep {

DrawSomething::DrawSomething(std::shared_ptr<Shader> shader): _shader(shader) {
	float positions[] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5f, 0.5f
	};

	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

void DrawSomething::render() {
	glUseProgram(_shader->rendererId);

	int location = glGetUniformLocation(_shader->rendererId, "u_Color");
	static int step = 2;
	static int i = 0;
	if (i > 255 || i < 0) {
		step = -step;
	}
	i += step;
	glUniform4f(location, i / 255.f, i / 255.f, 255 - i / 255.f, 1);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

}
