# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    ProjectTacticsMain.cpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_executable(ProjectTactics ${SOURCES})
target_link_libraries(ProjectTactics PUBLIC engine_application)

# Set the working directory for visual studio
set_target_properties(
    ProjectTactics PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:ProjectTactics>
)

# Copy all the dependencies ( dll ) to the executable file directory
add_custom_command(TARGET ProjectTactics POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:ProjectTactics> $<TARGET_RUNTIME_DLLS:ProjectTactics>
  COMMAND_EXPAND_LISTS
)
# Copy all the dependencies ( dll ) and the data folder to the executable file directory
add_custom_target(RunPostBuildCommand ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources/data $<TARGET_FILE_DIR:ProjectTactics>/data
  COMMAND_EXPAND_LISTS
)

# Make the post-build command depend on AnotherTarget
add_dependencies(RunPostBuildCommand framework_data)

# Make the main target depend on the post-build command target
add_dependencies(ProjectTactics RunPostBuildCommand)

unset(SOURCES)