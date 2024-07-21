#include "ClickModifiers.h"

namespace click {

void modify(NegateModifier& modifier, ActionValue& value) {
	switch (modifier.axis) {
	case Axis::X: value.vec3.x = -value.vec3.x; break;
	case Axis::Y: value.vec3.y = -value.vec3.y; break;
	case Axis::Z: value.vec3.z = -value.vec3.z; break;
	}
}

} // namespace click
