#pragma once

#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Utility/Math.h>

#include <Libs/Utility/Reflection.h>
#include <array>

namespace tactics::component {

struct BattleCamera {
	float rotationTime{};
	float rotationSpeed{};
	float distanceFromOrigin{};
	unsigned int nextStep{};
	unsigned int currentStep{};
	glm::vec3 offset{};
	std::vector<float> rotationSteps;

	float getCurrentRotationDegree() const;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(
		BattleCamera,
		rotationTime,
		distanceFromOrigin,
		rotationSpeed,
		nextStep,
		currentStep,
		rotationSteps,
		offset);
	static void defineReflection();
};

class BattleCameraSystem {
public:
	static void update(ecs_view<BattleCamera, Transform> view);
};

}
