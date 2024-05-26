#include "MaterialLoader.h"

namespace tactics::resource {
std::unique_ptr<Material> MaterialLoader::load(const nlohmann::json& /*descriptor*/) {
	return 0;
}
}
