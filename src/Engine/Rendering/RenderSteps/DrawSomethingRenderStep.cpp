#include "DrawSomethingRenderStep.h"

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Camera.h"

namespace tactics::renderstep {

struct Vertex {
	glm::vec2 position;
	glm::vec2 texCoord;
};

DrawSomething::DrawSomething(Shader* shader, Texture* texture)
	: _shader(shader)
	, _texture(texture) {
	float vertices[] = {
		-0.5f, -0.5f, -5.f, 0.0f, 0.0f,
		0.5f, -0.5f, -5.f, 1.0f, 0.0f,
		0.5f, 0.5f, -5.f, 1.0f, 1.0f,
		-0.5f, 0.5f, -5.f, 0.0f, 1.0f
	};

	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 4 * 5 * sizeof(float), vertices, GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	unsigned int ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);
}

void DrawSomething::execute(RenderStepInfo& renderInfo) {
	glUseProgram(_shader->rendererId);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, _texture->rendererId);
	glActiveTexture(GL_TEXTURE0);

	int mvpLocation = glGetUniformLocation(_shader->rendererId, "u_ModelViewProjection");
	int texLocation = glGetUniformLocation(_shader->rendererId, "u_Texture");

	glm::mat4 mvp = renderInfo.camera.getProjection() * renderInfo.camera.getView() * glm::mat4(1.0f);
	glUniform1i(texLocation, 0);
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

}
