#include "DrawSomethingRenderStep.h"

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Camera.h"

#include <Libs/Rendering/IndexBuffer.h>
#include <Libs/Rendering/VertexBuffer.h>
#include <Libs/Rendering/VertexDefinition.h>

namespace tactics::renderstep {

struct Vertex {
	glm::vec2 position;
	glm::vec2 texCoord;
};

VertexBuffer* vb;
IndexBuffer* ib;
VertexDefinition* vd;

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

	vb = new VertexBuffer();
	vb->setData(vertices, 4 * 5 * sizeof(float));

	auto builder = VertexDefinition::Builder();
	builder.attributef(3); // position
	builder.attributef(2); // uv
	vd = builder.create();
	vd->bind();

	ib = new IndexBuffer();
	ib->setData(indices, 6 * sizeof(GLuint));
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

	ib->bind();
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

}
