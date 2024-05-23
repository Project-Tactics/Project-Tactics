#include "DrawSomethingRenderStep.h"

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../Camera.h"

#include <Libs/Rendering/IndexBuffer.h>
#include <Libs/Rendering/VertexBuffer.h>
#include <Libs/Rendering/VertexDefinition.h>

namespace tactics::renderstep {

VertexDefinition* vd;
float angleX = 0.0f;

DrawSomething::DrawSomething(Shader& shader, Texture& texture, Mesh& mesh)
	: _shader(shader)
	, _texture(texture)
	, _mesh(mesh) {
	mesh.vertexBuffer->bind();
	auto builder = VertexDefinition::Builder();
	builder.attributef(3); // position
	builder.attributef(2); // uv
	vd = builder.create();
	vd->bind();
	mesh.vertexBuffer->unbind();
}

void DrawSomething::execute(RenderStepInfo& renderInfo) {
	glUseProgram(_shader.rendererId);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, _texture.rendererId);
	glActiveTexture(GL_TEXTURE0);

	int mvpLocation = glGetUniformLocation(_shader.rendererId, "u_ModelViewProjection");
	int texLocation = glGetUniformLocation(_shader.rendererId, "u_Texture");

	angleX += 0.025f;
	if (angleX > 2 * glm::pi<float>()) {
		angleX -= 2 * glm::pi<float>();
	}

	float distance = 40;
	glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 position = glm::vec3(distance * glm::cos(angleX), 0.0f, distance * glm::sin(angleX));

	auto& camera = renderInfo.camera;
	camera.setPosition(position + glm::vec3(0, 10, 0));
	camera.setDirection(center - position);

	glm::mat4 mvp = renderInfo.camera.getProjection() * renderInfo.camera.getView() * glm::mat4(1.0f);
	glUniform1i(texLocation, 0);
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, &mvp[0][0]);

	if (_mesh.indexBuffer->getSize() > 0) {
		_mesh.indexBuffer->bind();
		glDrawElements(GL_TRIANGLES, _mesh.indexBuffer->getSize(), GL_UNSIGNED_INT, nullptr);
	} else {
		glDrawArrays(GL_TRIANGLES, 0, _mesh.vertexBuffer->getSize());
	}
}

}
