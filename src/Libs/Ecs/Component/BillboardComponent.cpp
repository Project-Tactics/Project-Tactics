#include "BillboardComponent.h"

#include <Libs/Utility/Reflection.h>

namespace tactics::component {

void Billboard::defineReflection() {
	componentReflection<Billboard>("billboard");
}

}
