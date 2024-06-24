#include "CharacterFacing.h"

namespace tactics::component {

void CharacterFacing::defineReflection() {
	entt::meta<Facing>()
		.data<Facing::North>(hash("north"))
		.data<Facing::South>(hash("south"))
		.data<Facing::East>(hash("east"))
		.data<Facing::West>(hash("west"));

	componentReflection<CharacterFacing>("characterFacing")
		.data<&CharacterFacing::facing>(hash("facing"));
}

const std::array<std::array<Facing, 4>, 4> facingTransforms = {{
	{Facing::North, Facing::South, Facing::East, Facing::West},
	{Facing::West, Facing::East, Facing::North, Facing::South},
	{Facing::South, Facing::North, Facing::West, Facing::East},
	{Facing::East, Facing::West, Facing::South, Facing::North}
	}
};

void CharacterFacingSystem::update(ecs_view<Transform, Camera, CurrentCamera>& currentCameraView, ecs_view<CharacterFacing, SpriteAnimation, Sprite> characterFacingView) {
	auto [cameraEntity, cameraTransform, camera] = *currentCameraView.each().begin();

	auto origin = Vector3::zero;
	// TODO(Gerark) This is an hardcoded value based on what we have in data right now. It's going to break as soon as we change the rotateAroundPoint for camera
	// The best approach would be to have a proper custom component for our camera instead of using the generic RotateAroundPoint component
	// This is mostly working only for the demoMaps scene
	auto cameraAtZeroAngle = glm::vec3{10, 8, 0};
	auto cameraPos = cameraTransform.getPosition();
	if (cameraPos - origin == Vector3::zero) {
		return;
	}

	auto currentDir = glm::normalize(cameraPos - origin);
	auto cameraAtZeroAngleDir = glm::normalize(cameraAtZeroAngle - origin);
	auto crossProduct = glm::cross(cameraAtZeroAngleDir, currentDir);

	auto angle = glm::acos(glm::dot(cameraAtZeroAngleDir, currentDir));
	if (glm::dot(crossProduct, Vector3::up) < 0) {
		angle = -angle;
	}

	angle = glm::clamp(angle, -1.f, 1.f);
	angle = glm::degrees(angle);

	if (angle < 0) {
		angle += 360.0f;
	}

	auto index = static_cast<int>(glm::floor(angle / 90.f));

	printf("%.2f - ", angle);
	for (auto [entity, facing, animation, sprite] : characterFacingView.each()) {
		auto transformedFacing = facingTransforms[index][static_cast<int>(facing.facing)];

		switch (transformedFacing) {
		case Facing::North:
			sprite.spriteIndex = 3;
			sprite.uvFlip = {-1, 1};
			break;
		case Facing::South:
			sprite.spriteIndex = 1;
			sprite.uvFlip = {1, 1};
			break;
		case Facing::East:
			sprite.spriteIndex = 1;
			sprite.uvFlip = {-1, 1};
			break;
		case Facing::West:
			sprite.spriteIndex = 3;
			sprite.uvFlip = {1, 1};
			break;
		}
	}
}

}
