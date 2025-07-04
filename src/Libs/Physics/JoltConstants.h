#pragma once

// clang-format off
#include <Jolt/Jolt.h>
// clang-format on

#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayer.h>
#include <Jolt/Physics/Collision/ObjectLayer.h>
#include <array>

namespace tactics::physics {

// This is the max amount of rigid bodies that you can add to the physics system. If you try to add more you'll get an
// error.
static constexpr JPH::uint maxBodies = 1024;

// This determines how many mutexes to allocate to protect rigid bodies from concurrent access. Set it to 0 for the
// default settings.
static constexpr JPH::uint numBodyMutexes = 0;

// This is the max amount of body pairs that can be queued at any time (the broad phase will detect overlapping
// body pairs based on their bounding boxes and will insert them into a queue for the narrowphase). If you make this
// buffer too small the queue will fill up and the broad phase jobs will start to do narrow phase work. This is slightly
// less efficient. For a big project use something in the order of 65536.
static constexpr JPH::uint maxBodyPairs = 1024;

// This is the maximum size of the contact constraint buffer. If more contacts (collisions between bodies) are detected
// than this number then these contacts will be ignored and bodies will start interpenetrating / fall through the world.
// For a real big use something in the order of 10240.
static constexpr JPH::uint maxContactConstraints = 1024;

// Layers and Broad Phase Layers are mapped 1:1 and we have only Static and Dynamic layers atm.
static constexpr JPH::ObjectLayer LayerStatic{0};
static constexpr JPH::ObjectLayer LayerDynamic{1};
static constexpr JPH::ObjectLayer LayerCount{2};

static constexpr JPH::BroadPhaseLayer BPLayerStatic{0};
static constexpr JPH::BroadPhaseLayer BPLayerDynamic{1};
static constexpr JPH::uint BPLayerCount{2};

static constexpr std::array<JPH::BroadPhaseLayer, physics::LayerCount> LayerConversionTable = {BPLayerStatic,
																							   BPLayerDynamic};

} // namespace tactics::physics
