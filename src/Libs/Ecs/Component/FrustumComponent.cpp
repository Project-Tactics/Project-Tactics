#include "FrustumComponent.h"

#include "../EntityUtilities.h"
#include "../Entity.h"

namespace tactics::component {

void Frustum::defineReflection() {
	using namespace entt::literals;

	entt::meta<Frustum>()
		.type(hash("frustum"))
		.func<&Entity::explicitAddComponent<Frustum>>(hash("emplace"))
		.data<&Frustum::fov>(hash("fov"))
		.data<&Frustum::near>(hash("near"))
		.data<&Frustum::far>(hash("far"))
		.data<&Frustum::aspectRatio>(hash("aspectRatio"))
		.data<&Frustum::orthoSize>(hash("orthoSize"));
}

}
