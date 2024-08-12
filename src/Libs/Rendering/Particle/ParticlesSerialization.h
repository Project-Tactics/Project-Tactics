#pragma once

#include <Libs/Rendering/Particle/Particles.h>
#include <Libs/Utility/Json/JsonSerialization.h>

namespace firebolt {

JSON_VARIANT_NAME(firebolt::PointSpawnPosition, "point");
JSON_VARIANT_NAME(firebolt::CircleSpawnPosition, "circle");
JSON_VARIANT_NAME(firebolt::ConeSpawnPosition, "cone");

JSON_VARIANT_NAME(firebolt::ColorOverLifetime, "colorOverLifetime");
JSON_VARIANT_NAME(firebolt::SizeOverLifetime, "sizeOverLifetime");

JSON_SERIALIZE_EXT(firebolt::PointSpawnPosition, position);
JSON_SERIALIZE_EXT(firebolt::CircleSpawnPosition, position, radius);
JSON_SERIALIZE_EXT(firebolt::ConeSpawnPosition, position, radius, angle);

JSON_SERIALIZE_EXT(firebolt::ColorOverLifetime, startColor, endColor);
JSON_SERIALIZE_EXT(firebolt::SizeOverLifetime, startSize, endSize);

JSON_SERIALIZE_EXT(firebolt::EffectConfig, spawnPosition, lifetime, speed, size, color, emitRate, modifiers);

} // namespace firebolt
