#include "ParticleEffectLoader.h"

namespace tactics::resource {

std::shared_ptr<ParticleEffect> ParticleEffectLoader::load(const ParticleEffectDescriptor& descriptor) {
	auto effect = std::make_shared<ParticleEffect>();
	effect->config = descriptor.config;
	effect->shader = descriptor.shader;
	effect->texture = descriptor.texture;
	return effect;
}

} // namespace tactics::resource
