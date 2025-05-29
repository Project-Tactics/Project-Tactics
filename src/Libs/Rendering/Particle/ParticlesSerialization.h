#pragma once

#include <Libs/Rendering/Particle/Particles.h>
#include <Libs/Utility/Json/JsonSerialization.h>

namespace firebolt {

JSON_VARIANT_NAME(firebolt::SphereEmitShape, "sphere");
JSON_VARIANT_NAME(firebolt::ConeEmitShape, "cone");

JSON_VARIANT_NAME(firebolt::ColorOverLifetime, "colorOverLifetime");
JSON_VARIANT_NAME(firebolt::SizeOverLifetime, "sizeOverLifetime");

JSON_VARIANT_NAME(firebolt::FixedValue, "fixed");
JSON_VARIANT_NAME(firebolt::RandomRange, "randomRange");

JSON_SERIALIZE_EXT(firebolt::SphereEmitShape, position, radius);
JSON_SERIALIZE_EXT(firebolt::ConeEmitShape, position, radius);

JSON_SERIALIZE_EXT(firebolt::ColorOverLifetime, startColor, endColor);
JSON_SERIALIZE_EXT(firebolt::SizeOverLifetime, startSize, endSize);

JSON_SERIALIZE_EXT(firebolt::FixedValue, value);
JSON_SERIALIZE_EXT(firebolt::RandomRange, min, max);

JSON_SERIALIZE_EXT(firebolt::EffectConfig, emitShape, startLifetime, startSpeed, startSize, startRotation, color, emitRate, updaters);

} // namespace firebolt
