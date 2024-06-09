#include "FrustumComponent.h"

#include <Libs/Utility/Reflection.h>

namespace tactics::component {

void Frustum::defineReflection() {
	using namespace entt::literals;

	componentReflection<Frustum>("frustum")
		.data<&Frustum::fov>(hash("fov"))
		.data<&Frustum::near>(hash("near"))
		.data<&Frustum::far>(hash("far"))
		.data<&Frustum::aspectRatio>(hash("aspectRatio"))
		.data<&Frustum::orthoSize>(hash("orthoSize"));
}

}
