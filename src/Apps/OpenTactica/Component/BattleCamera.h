#pragma once

#include <Libs/Ecs/EntityUtilities.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Utility/Math.h>

#include <Libs/Utility/Reflection.h>
#include <array>

namespace tactics::component {

struct BattleCamera {
	float currentRotation{};
	float rotationSpeed{};
	unsigned int nextStep{};
	unsigned int currentStep{};
	glm::vec3 startingPosition = Vector3::zero;
	std::vector<float> rotationSteps;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE_WITH_DEFAULT(BattleCamera, currentRotation, rotationSpeed, nextStep, currentStep, startingPosition, rotationSteps);
	static void defineReflection();
};

class BattleCameraSystem {
public:
	static void update(ecs_view<BattleCamera, Transform> view);
};

}
