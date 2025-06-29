#include "DrawDebugRenderStep.h"

#include <Libs/Ecs/Component/DebugDrawingComponent.h>
#include <Libs/Ecs/EntityComponentSystem.h>

#include <glad/gl.h>

namespace tactics::renderstep {

DrawDebug::DrawDebug(EntityComponentSystem& ecs, resource::Shader::Ptr shader) : _ecs(ecs), _shader(shader) {
	_generateUnitSphereLines();
	_generateUnitBoxLines();

	// let's use the sphere complexity to determine the size of the buffer
	_vertices.resize(_unitSphereLines.size() * 50);

	_vb.generateData(_vertices, rp::DynamicDraw::value);

	_vb.bind();
	VertexAttributes::Builder builder;
	builder.attributef(3); // position
	builder.attributef(4); // color
	builder.create(_va);
	_vb.unbind();
}

void DrawDebug::execute(RenderStepInfo& renderInfo) {
	using namespace component;

	auto vertexCount = 0;

	// Fill Vertex Info for Lines
	{
		auto view = _ecs.sceneRegistry().view<DebugLine>();

		for (auto&& [entity, line] : view.each()) {
			_vertices[vertexCount] = {line.start, line.color};
			_vertices[vertexCount + 1] = {line.end, line.color};
			vertexCount += 2;

			if (vertexCount + 2 >= _vertices.size()) {
				break;
			}
		}
	}

	// Fill Vertex Info for Spheres
	{
		if (vertexCount + _unitSphereLines.size() * 2 < _vertices.size()) {
			auto view = _ecs.sceneRegistry().view<DebugSphere>();
			for (auto&& [entity, sphere] : view.each()) {
				for (auto& [start, end] : _unitSphereLines) {
					glm::vec3 a = sphere.position + start * sphere.radius;
					glm::vec3 b = sphere.position + end * sphere.radius;

					if (vertexCount + 2 >= _vertices.size()) {
						break;
					}

					_vertices[vertexCount++] = {a, sphere.color};
					_vertices[vertexCount++] = {b, sphere.color};
				}

				if (vertexCount + _unitSphereLines.size() * 2 >= _vertices.size()) {
					break;
				}
			}
		}
	}

	// Fill Vertex Info for Boxes
	{
		if (vertexCount + _unitBoxLines.size() * 2 < _vertices.size()) {
			auto view = _ecs.sceneRegistry().view<DebugBox>();
			for (auto&& [entity, box] : view.each()) {
				for (auto& [start, end] : _unitBoxLines) {
					glm::vec3 a = box.position + start * box.size;
					glm::vec3 b = box.position + end * box.size;
					if (vertexCount + 2 >= _vertices.size()) {
						break;
					}
					_vertices[vertexCount++] = {a, box.color};
					_vertices[vertexCount++] = {b, box.color};
				}
				if (vertexCount + _unitBoxLines.size() * 2 >= _vertices.size()) {
					break;
				}
			}
		}
	}

	if (vertexCount <= 0) {
		return;
	}

	_vb.updateData(_vertices, 0);

	_va.bind();

	_shader->bind();
	_shader->setUniform("u_ModelViewProjection", renderInfo.viewProjection);

	_vb.bind();
	glDrawArrays(GL_LINES, 0, vertexCount);
	_va.unbind();
	_vb.unbind();

	glUseProgram(0); // Unbind shader program
}

void DrawDebug::_generateUnitSphereLines() {
	const int rings = 12;
	const int segments = 24;

	_unitSphereLines.clear();

	// Horizontal rings
	for (int i = 0; i < rings; ++i) {
		float theta = i * Math::PI / rings;
		for (int j = 0; j < segments; ++j) {
			float phi = j * 2.0f * Math::PI / segments;
			float nextPhi = (j + 1) * 2.0f * Math::PI / segments;

			glm::vec3 p1 = {std::sin(theta) * std::cos(phi), std::cos(theta), std::sin(theta) * std::sin(phi)};
			glm::vec3 p2 = {std::sin(theta) * std::cos(nextPhi), std::cos(theta), std::sin(theta) * std::sin(nextPhi)};
			_unitSphereLines.emplace_back(p1, p2);
		}
	}

	// Vertical rings
	for (int j = 0; j < segments; ++j) {
		float phi = j * 2.0f * Math::PI / segments;
		for (int i = 0; i < rings; ++i) {
			float theta = i * Math::PI / rings;
			float nextTheta = (i + 1) * Math::PI / rings;

			glm::vec3 p1 = {std::sin(theta) * std::cos(phi), std::cos(theta), std::sin(theta) * std::sin(phi)};
			glm::vec3 p2 = {std::sin(nextTheta) * std::cos(phi),
							std::cos(nextTheta),
							std::sin(nextTheta) * std::sin(phi)};
			_unitSphereLines.emplace_back(p1, p2);
		}
	}
}

void DrawDebug::_generateUnitBoxLines() {
	glm::vec3 corners[8] =
		{{-1, -1, -1}, {1, -1, -1}, {1, 1, -1}, {-1, 1, -1}, {-1, -1, 1}, {1, -1, 1}, {1, 1, 1}, {-1, 1, 1}};

	int edges[12][2] = {
		{0, 1},
		{1, 2},
		{2, 3},
		{3, 0}, // bottom
		{4, 5},
		{5, 6},
		{6, 7},
		{7, 4}, // top
		{0, 4},
		{1, 5},
		{2, 6},
		{3, 7} // vertical
	};

	for (auto& e : edges) {
		_unitBoxLines.emplace_back(corners[e[0]] * 0.5f, corners[e[1]] * 0.5f);
	}
}

} // namespace tactics::renderstep
