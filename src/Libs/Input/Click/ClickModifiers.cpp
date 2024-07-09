#include "ClickModifiers.h"

namespace click::negate {

void modify(Modifier& /*modifier*/, ActionValue& value) {
	value.axis3D.x = -value.axis3D.x;
	value.axis3D.y = -value.axis3D.y;
	value.axis3D.z = -value.axis3D.z;
}

} // namespace click::negate

namespace click::toAxis {

void modify(Modifier& modifier, ActionValue& value) {
	switch (modifier.data.toAxis.axis) {
	case Axis::XYZ: {
		value.axis3D = value.axis3D;
		break;
	}
	case Axis::YZX: {
		auto y = value.axis3D.x;
		auto z = value.axis3D.y;
		auto x = value.axis3D.z;
		value.axis3D.x = x;
		value.axis3D.y = y;
		value.axis3D.z = z;
		break;
	}
	case Axis::ZXY: {
		auto z = value.axis3D.x;
		auto x = value.axis3D.y;
		auto y = value.axis3D.z;
		value.axis3D.x = x;
		value.axis3D.y = y;
		value.axis3D.z = z;
		break;
	}
	}
}

} // namespace click::toAxis
