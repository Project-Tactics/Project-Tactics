#include "ClickModifiers.h"

namespace click::negate {

void modify(Modifier& /*modifier*/, ActionValue& value) {
	value.axis3D.x = -value.axis3D.x;
	value.axis3D.y = -value.axis3D.y;
	value.axis3D.z = -value.axis3D.z;
}

} // namespace click::negate
