#pragma once

#include <Libs/Rendering/Particle/Particles.h>
#include <Libs/Resource/Resource.h>
#include <Libs/Resource/Shader/Shader.h>
#include <Libs/Resource/Texture/Texture.h>

namespace tactics::resource {

class ParticleEffect : public Resource<ParticleEffect> {
public:
	static const ResourceType TYPE = ResourceType::ParticleEffect;
	using Resource<ParticleEffect>::Resource;

	firebolt::EffectConfig config;
	resource::Shader::Ptr shader;
	resource::Texture::Ptr texture;
};

} // namespace tactics::resource
