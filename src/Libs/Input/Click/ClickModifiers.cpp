#include "ClickModifiers.h"

namespace click::negate {

void modify(Modifier& /*modifier*/, ActionValue& value) {
	value.vec3.x = -value.vec3.x;
	value.vec3.y = -value.vec3.y;
	value.vec3.z = -value.vec3.z;
}

} // namespace click::negate

