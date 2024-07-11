#include "ClickModifiers.h"

namespace click::negate {

void modify(Modifier& /*modifier*/, ActionValue& value) {
	value.vec3.x = -value.vec3.x;
	value.vec3.y = -value.vec3.y;
	value.vec3.z = -value.vec3.z;
}

} // namespace click::negate

namespace click::toAxis {

void modify(Modifier& modifier, ActionValue& value) {
	switch (modifier.data.toAxis.axis) {
	case Axis::XYZ: {
		value.vec3 = value.vec3;
		break;
	}
	case Axis::YZX: {
		auto y = value.vec3.x;
		auto z = value.vec3.y;
		auto x = value.vec3.z;
		value.vec3.x = x;
		value.vec3.y = y;
		value.vec3.z = z;
		break;
	}
	case Axis::ZXY: {
		auto z = value.vec3.x;
		auto x = value.vec3.y;
		auto y = value.vec3.z;
		value.vec3.x = x;
		value.vec3.y = y;
		value.vec3.z = z;
		break;
	}
	}
}

} // namespace click::toAxis
