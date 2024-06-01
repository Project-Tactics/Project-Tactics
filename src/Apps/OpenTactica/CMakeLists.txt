# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    OpenTacticaApplication.cpp
    main.cpp
    States/DemoState.cpp
    States/EmptyState.cpp
    States/LoadState.cpp
    States/MapState.cpp
    Component/RotateAroundPoint.cpp
    Component/RotateItem.cpp)
    
set(HEADERS
    OpenTacticaApplication.h
    States/DemoState.h
    States/EmptyState.h
    States/LoadState.h
    States/MapState.h
    Component/RotateAroundPoint.h
    Component/RotateItem.h)

set(TARGET_NAME OpenTactica)
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
add_custom_target(RunPostBuildCommand ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources/data $<TARGET_FILE_DIR:${TARGET_NAME}>/data
  COMMAND_EXPAND_LISTS
)

# Make the post-build command depend on AnotherTarget
add_dependencies(RunPostBuildCommand framework_data)

# Make the main target depend on the post-build command target
add_dependencies(${TARGET_NAME} RunPostBuildCommand)

set_property(TARGET RunPostBuildCommand PROPERTY FOLDER "_cmakeAndPostBuild")

unset(SOURCES)
unset(HEADERS)
unset(TARGET_NAME)