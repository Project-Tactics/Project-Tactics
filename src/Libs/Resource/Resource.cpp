#include "Resource.h"

#include <Libs/Utility/Exception.h>
#include <Libs/Utility/UUID.h>

namespace tactics::resource {

BaseResource::BaseResource(HashId name, ResourceType type) {
	this->id = generateUUID();
	this->name = name;
	this->type = type;
}

} // namespace tactics::resource
