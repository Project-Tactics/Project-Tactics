#pragma once

#include "ParticleEffect.h"

#include <Libs/Rendering/Particle/ParticlesSerialization.h>
#include <Libs/Resource/ResourceLoader.h>
#include <Libs/Resource/ResourceSerialization.h>

namespace tactics::resource {

struct ParticleEffectDescriptor {
	firebolt::EffectConfig config;
	resource::Shader::Ptr shader;
	resource::Texture::Ptr texture;

	JSON_SERIALIZE(ParticleEffectDescriptor, config, shader, texture);
};

class ParticleEffectLoader : public ResourceLoader {
public:
	using ResourceLoader::ResourceLoader;
	[[nodiscard]] std::shared_ptr<ParticleEffect> load(const ParticleEffectDescriptor& descriptor);
};

} // namespace tactics::resource
