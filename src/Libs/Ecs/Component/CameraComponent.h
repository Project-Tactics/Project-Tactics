#pragma once

#include <Libs/Utility/Reflection.h>

#include <glm/glm.hpp>

namespace tactics::component {

enum ProjectionType {
	Perspective,
	Orthographic
};

NLOHMANN_JSON_SERIALIZE_ENUM(ProjectionType,
							 {{ProjectionType::Perspective, "perspective"},
							  {ProjectionType::Orthographic, "orthographic"}});

struct Camera {
	ProjectionType projectionType = ProjectionType::Perspective;
	glm::mat4 projection;
	glm::mat4 view;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(Camera, projectionType, projection, view);
	static void defineReflection();
	Camera clone();
};

struct CurrentCamera {
	static void defineReflection();
};

} // namespace tactics::component
