#pragma once

namespace tactics {

template<typename ...TArgs>
void defineComponentsReflection() {
	(TArgs::defineReflection(), ...);
}

}
