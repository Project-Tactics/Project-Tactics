cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    Entity.cpp
    EntityComponentSystem.cpp
    EntityUtilities.cpp
    Component/MeshComponent.cpp
    SubSystem/CameraSubSystem.cpp)
    
set(HEADERS
    Entity.h
    EntityComponentSystem.h
    EntityUtilities.h
    Component/AlphaBlendedComponent.h
    Component/CameraComponent.h
    Component/FrustumComponent.h
    Component/MeshComponent.h
    Component/NameComponent.h
    Component/TransformComponent.h
    Component/ViewportComponent.h
    SubSystem/EntityComponentSubSystem.h
    SubSystem/CameraSubSystem.h)

add_library(libs_ecs STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_ecs PUBLIC entt glad libs_utility)
add_to_libs_folder(libs_ecs)

set(TEST_SOURCES Tests/Test_EntityComponentSystem.cpp)
set(TEST_LIBS libs_ecs)
add_unit_test(libs_ecs ${TEST_SOURCES} ${TEST_LIBS})

unset(SOURCES)
unset(HEADERS)
