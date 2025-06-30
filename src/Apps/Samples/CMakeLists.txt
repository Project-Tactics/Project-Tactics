# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    Component/BallBouncing.cpp
    Component/BallMovement.cpp
    Component/BattleCamera.cpp
    Component/CharacterFacing.cpp
    Component/FreeCamera.cpp
    Component/PlayerMovement.cpp
    Component/Rectangle2DCollider.cpp
    Component/RotateAroundPoint.cpp
    Component/RotateItem.cpp
    Component/TranslateItem.cpp
    States/DemoPongState.cpp
    States/DemoUserInterfaceState.cpp
    States/DemoDebugDrawingState.cpp
    main.cpp
    Overlay/SampleSelectionOverlay.cpp
    SamplesApplication.cpp
    States/DemoMapState.cpp
    States/DemoParticlesState.cpp
    States/DemoSimpleState.cpp
    States/DemoSpriteState.cpp
    States/EmptyState.cpp
    States/LoadState.cpp
    States/SampleSelectionState.cpp
    States/SetupState.cpp
    States/UnloadState.cpp)
    
set(HEADERS
    Component/BallBouncing.h
    Component/BallMovement.h
    Component/BattleCamera.h
    Component/CharacterFacing.h
    Component/FreeCamera.h
    Component/PlayerMovement.cpp
    Component/Rectangle2DCollider.cpp
    Component/RotateAroundPoint.h
    Component/RotateItem.h
    Component/TranslateItem.h
    Overlay/SampleSelectionOverlay.h
    SamplesApplication.h
    States/DemoPongState.h
    States/DemoUserInterfaceState.h
    States/DemoDebugDrawingState.h
    SamplesUtils.h
    States/DemoMapState.h
    States/DemoParticlesState.h
    States/DemoSimpleState.h
    States/DemoSpriteState.h
    States/EmptyState.h
    States/LoadState.h
    States/SampleSelectionState.h
    States/SetupState.h
    States/UnloadState.h)

set(TARGET_NAME Samples)
set(POST_BUILD_TARGET_NAME ${TARGET_NAME}PostBuild)
set(DATA_TARGET_NAME samples_data)
add_executable(${TARGET_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${TARGET_NAME} PUBLIC tactics_engine)
add_to_apps_folder(${TARGET_NAME})

# Set the working directory for visual studio
set_target_properties(
    ${TARGET_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${TARGET_NAME}>
)

# Copy all the dependencies ( dll ) to the executable file directory
add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${TARGET_NAME}> $<TARGET_RUNTIME_DLLS:${TARGET_NAME}>
  COMMAND_EXPAND_LISTS
)
# Copy all the dependencies ( dll ) and the data folder to the executable file directory
add_custom_target(${POST_BUILD_TARGET_NAME} ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources/${TARGET_NAME}/data $<TARGET_FILE_DIR:${TARGET_NAME}>/data
  COMMAND_EXPAND_LISTS
)

# Make the post-build command depend on data target
add_dependencies(${POST_BUILD_TARGET_NAME} ${DATA_TARGET_NAME})

# Make the main target depend on the post-build command target
add_dependencies(${TARGET_NAME} ${POST_BUILD_TARGET_NAME})

set_property(TARGET ${POST_BUILD_TARGET_NAME} PROPERTY FOLDER "_cmakeAndPostBuild")

unset(SOURCES)
unset(HEADERS)
unset(TARGET_NAME)