#pragma once

#include "ParticlesTypes.h"

namespace firebolt {

void init(unsigned int maxParticleSize);
void shutdown();
void update(float deltaTime);

EffectId createEffect(const EffectConfig& config);
void destroyEffect(EffectId effectId);
bool isEffectValid(EffectId effectId);
Effect& getEffect(EffectId effectId);
unsigned int getEffectsCount();

void emitParticle(Effect& effect, const EffectConfig& config);

const EffectConfig& getConfig(EffectId effectId);
void updateConfig(EffectId effectId, const EffectConfig& config);

} // namespace firebolt
