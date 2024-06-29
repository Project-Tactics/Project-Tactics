#include "FrustumComponent.h"

#include <Libs/Utility/Reflection.h>

namespace tactics::component {

void Frustum::defineReflection() {
	using namespace entt::literals;

	componentReflection<Frustum>("frustum")
		.data<&Frustum::fov>("fov"_id)
		.data<&Frustum::near>("near"_id)
		.data<&Frustum::far>("far"_id)
		.data<&Frustum::aspectRatio>("aspectRatio"_id)
		.data<&Frustum::orthoSize>("orthoSize"_id);
}

}
