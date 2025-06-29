#pragma once

#include <Libs/Resource/ParticleEffect/ParticleEffect.h>
#include <Libs/Utility/Reflection.h>

#include <glm/glm.hpp>

namespace tactics::component {

struct ParticleEmitter {
	resource::ParticleEffect::Ptr effectResource;
	std::optional<firebolt::EffectId> maybeEffectId;
	bool isEmittingParticles{false};

	COMPONENT(ParticleEmitter, effectResource, isEmittingParticles)
};

struct ParticleEmitterPlaying {};

} // namespace tactics::component

