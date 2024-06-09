#include "Prefab.h"

#include <Libs/Ecs/Entity.h>

namespace tactics::resource {

Prefab::~Prefab() {
	entity.destroy();
}

}
