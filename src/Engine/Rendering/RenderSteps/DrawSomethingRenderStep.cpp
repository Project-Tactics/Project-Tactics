#include "DrawSomethingRenderStep.h"

#include <glad/gl.h>
#include <glm/glm.hpp>

namespace tactics::renderstep {

struct Vertex {
	glm::vec2 position;
	glm::vec2 texCoord;
};

DrawSomething::DrawSomething(std::shared_ptr<Shader> shader, std::shared_ptr<Texture> texture)
	: _shader(shader)
	, _texture(texture) {
	float vertices[] = {
		-0.25f, -0.5f, 0.0f, 0.0f,
		0.25f, -0.5f, 1.0f, 0.0f,
		0.25f, 0.5f, 1.0f, 1.0f,
		-0.25f, 0.5f, 0.0f, 1.0f
	};

	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * 4 * sizeof(float), vertices, GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(2 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

void DrawSomething::render() {
	glUseProgram(_shader->rendererId);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, _texture->rendererId);
	glActiveTexture(GL_TEXTURE0);

	int texLocation = glGetUniformLocation(_shader->rendererId, "u_Texture");
	int colLocation = glGetUniformLocation(_shader->rendererId, "u_Color");
	static int step = 2;
	static int i = 0;
	if (i > 255 || i < 0) {
		step = -step;
	}
	i += step;
	glUniform4f(colLocation, i / 255.f, i / 255.f, 255 - i / 255.f, 1);
	glUniform1i(texLocation, 0);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

}
