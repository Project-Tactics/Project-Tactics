#include <Engine/Scene/SceneSystem.h>

#include <Libs/Ecs/Component/AlphaBlendedComponent.h>
#include <Libs/Ecs/Component/CameraComponent.h>
#include <Libs/Ecs/Component/MeshComponent.h>
#include <Libs/Ecs/Component/ParticleEmitterComponent.h>
#include <Libs/Ecs/Component/PhysicsComponent.h>
#include <Libs/Ecs/Component/RenderableComponent.h>
#include <Libs/Ecs/Component/SpriteComponent.h>
#include <Libs/Ecs/Component/TransformComponent.h>
#include <Libs/Ecs/EntityComponentSystem.h>
#include <Libs/FileSystem/FileLoader.h>
#include <Libs/FileSystem/FileSystem.h>
#include <Libs/Physics/PhysicsSystem.h>
#include <Libs/Rendering/Particle/ParticleSystem.h>
#include <Libs/Resource/ResourceSystem.h>

#include <gtest/gtest.h>
#include <memory>
#include <type_traits>

namespace tactics {
namespace {

static_assert(!std::is_copy_constructible_v<SceneSystem> && !std::is_move_constructible_v<SceneSystem>);
static_assert(!std::is_copy_constructible_v<PhysicsSystem> && !std::is_move_constructible_v<PhysicsSystem>);
static_assert(!std::is_copy_constructible_v<ParticleSystem> && !std::is_move_constructible_v<ParticleSystem>);

struct RegistryObserver {
	int calls{};

	void onEvent(entt::registry&, entt::entity) {
		++calls;
	}
};

class SystemSubscriptionsTest : public testing::Test {
protected:
	void SetUp() override {
		auto dataPath = std::make_unique<PathHelper>("");
		auto fileLoader = std::make_unique<DefaultFileLoader>(*dataPath);
		_fileSystem = std::make_unique<FileSystem>(std::move(fileLoader), std::move(dataPath));
		_resources = std::make_unique<resource::ResourceSystem>(*_fileSystem);
	}

	EntityComponentSystem _ecs;
	std::unique_ptr<FileSystem> _fileSystem;
	std::unique_ptr<resource::ResourceSystem> _resources;
};

TEST_F(SystemSubscriptionsTest, SceneCallbacksStopAfterDestructionAndPreserveOtherListeners) {
	using namespace component;
	auto& registry = _ecs.sceneRegistry();
	RegistryObserver observer;
	entt::scoped_connection cameraConnection =
		registry.on_construct<CurrentCamera>().connect<&RegistryObserver::onEvent>(observer);

	const auto firstCamera = registry.create();
	const auto secondCamera = registry.create();
	const auto renderable = registry.create();
	{
		SceneSystem system(_ecs, *_resources);
		registry.emplace<CurrentCamera>(firstCamera);
		registry.emplace<CurrentCamera>(secondCamera);
		EXPECT_FALSE(registry.all_of<CurrentCamera>(firstCamera));
		EXPECT_TRUE(registry.all_of<CurrentCamera>(secondCamera));

		registry.emplace<Renderable>(renderable, RenderType::Particle);
		EXPECT_TRUE((registry.all_of<AlphaBlended, FullyAlphaBlended>(renderable)));
		registry.remove<AlphaBlended, FullyAlphaBlended>(renderable);
		registry.patch<Renderable>(renderable);
		EXPECT_TRUE((registry.all_of<AlphaBlended, FullyAlphaBlended>(renderable)));
	}

	EXPECT_TRUE(registry.on_construct<Mesh>().empty());
	EXPECT_TRUE(registry.on_destroy<Mesh>().empty());
	EXPECT_TRUE(registry.on_construct<Renderable>().empty());
	EXPECT_TRUE(registry.on_update<Renderable>().empty());
	EXPECT_TRUE(registry.on_construct<SpriteAnimation>().empty());
	EXPECT_TRUE(registry.on_update<SpriteAnimation>().empty());
	EXPECT_FALSE(registry.on_construct<CurrentCamera>().empty());

	const auto thirdCamera = registry.create();
	registry.emplace<CurrentCamera>(thirdCamera);
	EXPECT_TRUE(registry.all_of<CurrentCamera>(secondCamera));
	EXPECT_TRUE(registry.all_of<CurrentCamera>(thirdCamera));
	EXPECT_EQ(observer.calls, 3);

	registry.remove<AlphaBlended, FullyAlphaBlended>(renderable);
	registry.patch<Renderable>(renderable);
	EXPECT_FALSE((registry.any_of<AlphaBlended, FullyAlphaBlended>(renderable)));
	const auto detachedEntity = registry.create();
	registry.emplace<Renderable>(detachedEntity, RenderType::Particle);
	EXPECT_FALSE((registry.any_of<AlphaBlended, FullyAlphaBlended>(detachedEntity)));
	registry.remove<Renderable>(detachedEntity);

	// Without a scene subscriber, empty resource pointers must not trigger rendering callbacks.
	registry.emplace<Mesh>(detachedEntity);
	EXPECT_FALSE(registry.all_of<Renderable>(detachedEntity));
	registry.remove<Mesh>(detachedEntity);
	registry.emplace<SpriteAnimation>(detachedEntity);
	registry.patch<SpriteAnimation>(detachedEntity);
}

TEST_F(SystemSubscriptionsTest, PhysicsCallbacksStopAfterDestructionAndPreserveOtherListeners) {
	using namespace component;
	auto& registry = _ecs.sceneRegistry();
	RegistryObserver observer;
	entt::scoped_connection shapeConnection =
		registry.on_construct<BoxShape>().connect<&RegistryObserver::onEvent>(observer);
	const auto entity = registry.create();
	registry.emplace<Transform>(entity);
	{
		PhysicsSystem system(1 * 1024 * 1024, _ecs);
		registry.emplace<BoxShape>(entity);
		registry.remove<BoxShape>(entity);
		EXPECT_EQ(observer.calls, 1);
	}

	EXPECT_FALSE(registry.on_construct<BoxShape>().empty());
	EXPECT_TRUE(registry.on_destroy<BoxShape>().empty());
	EXPECT_TRUE(registry.on_construct<PhysicsBody>().empty());
	EXPECT_TRUE(registry.on_destroy<PhysicsBody>().empty());
	EXPECT_TRUE(registry.on_update<Transform>().empty());

	auto& body = registry.emplace<PhysicsBody>(entity);
	// A sentinel proves that later registry mutations do not reach a destroyed Jolt backend.
	body.bodyId = 1234;
	registry.emplace<BoxShape>(entity);
	EXPECT_EQ(body.bodyId, 1234u);
	registry.patch<Transform>(entity);
	EXPECT_EQ(body.bodyId, 1234u);
	registry.remove<BoxShape>(entity);
	EXPECT_EQ(body.bodyId, 1234u);
	registry.remove<PhysicsBody>(entity);
	EXPECT_EQ(observer.calls, 2);
}

TEST_F(SystemSubscriptionsTest, ParticleCallbacksStopAfterDestructionAndPreserveOtherListeners) {
	using namespace component;
	auto& registry = _ecs.sceneRegistry();
	RegistryObserver observer;
	entt::scoped_connection constructConnection =
		registry.on_construct<ParticleEmitter>().connect<&RegistryObserver::onEvent>(observer);
	entt::scoped_connection destroyConnection =
		registry.on_destroy<ParticleEmitter>().connect<&RegistryObserver::onEvent>(observer);
	const auto entity = registry.create();
	{
		ParticleSystem system(*_resources, _ecs);
		auto effect = std::make_shared<resource::ParticleEffect>();
		effect->config.emitRate = 1.0f;
		ParticleEmitter emitter;
		emitter.effectResource = effect;
		registry.emplace<ParticleEmitter>(entity, emitter);
		EXPECT_TRUE(registry.get<ParticleEmitter>(entity).maybeEffectId.has_value());
		EXPECT_TRUE(registry.all_of<Renderable>(entity));
		EXPECT_EQ(system.getEffectsCount(), 1u);
		registry.remove<ParticleEmitter>(entity);
		EXPECT_FALSE(registry.all_of<Renderable>(entity));
	}

	registry.emplace<ParticleEmitter>(entity);
	EXPECT_FALSE(registry.get<ParticleEmitter>(entity).maybeEffectId.has_value());
	EXPECT_FALSE(registry.all_of<Renderable>(entity));
	registry.remove<ParticleEmitter>(entity);
	EXPECT_EQ(observer.calls, 4);
}

} // namespace
} // namespace tactics
