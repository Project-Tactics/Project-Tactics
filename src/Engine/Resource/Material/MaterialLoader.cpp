#include "MaterialLoader.h"

namespace tactics::resource {
std::shared_ptr<Material> MaterialLoader::load(const nlohmann::json& /*descriptor*/) {
	return nullptr;
}
}
