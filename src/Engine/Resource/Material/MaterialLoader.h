#pragma once

#include <Libs/Resource/ResourceLoader.h>
#include "Material.h"

namespace tactics::resource {

class MaterialLoader: public ResourceLoader {
public:
	std::shared_ptr<Material> load(const nlohmann::json& descriptor);
};

}
