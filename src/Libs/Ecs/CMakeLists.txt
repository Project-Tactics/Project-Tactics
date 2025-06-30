cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    Component/BillboardComponent.cpp
    Component/CameraComponent.cpp
    Component/ComponentReflection.cpp
    Component/DebugDrawingComponent.cpp
    Component/FrustumComponent.cpp
    Component/MeshComponent.cpp
    Component/ParticleEmitterComponent.cpp
    Component/PrefabComponent.cpp
    Component/RenderableComponent.cpp
    Component/SpriteComponent.cpp
    Component/TransformComponent.cpp
    Entity.cpp
    EntityComponentSystem.cpp
    EntityUtilities.cpp
    PrefabManager.cpp
    System/BillboardSystem.cpp
    System/CameraSystem.cpp
    System/DebugDrawingSystem.cpp
    System/SpriteSystem.cpp
    System/TransformSystem.cpp)
    
set(HEADERS
    Component/AlphaBlendedComponent.h
    Component/BillboardComponent.h
    Component/CameraComponent.h
    Component/ComponentReflection.h
    Component/DebugDrawingComponent.h
    Component/FrustumComponent.h
    Component/MeshComponent.h
    Component/NameComponent.h
    Component/ParticleEmitterComponent.h
    Component/PrefabComponent.h
    Component/RenderableComponent.h
    Component/SpriteComponent.h 
    Component/TransformComponent.h
    Entity.h
    Entity.inl
    EntityComponentSystem.h
    EntityUtilities.h
    PrefabManager.h
    System/BillboardSystem.h
    System/CameraSystem.h
    System/DebugDrawingSystem.h
    System/SpriteSystem.h
    System/TransformSystem.h)

add_library(libs_ecs STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_ecs PUBLIC entt glad libs_utility)
add_to_libs_folder(libs_ecs)

set(TEST_SOURCES Tests/Test_EntityComponentSystem.cpp)
set(TEST_LIBS libs_ecs)
add_unit_test(libs_ecs ${TEST_SOURCES} ${TEST_LIBS})

unset(SOURCES)
unset(HEADERS)
