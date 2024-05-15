# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    ProjectTacticsMain.cpp)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_executable(ProjectTactics ${SOURCES})
target_link_libraries(ProjectTactics PUBLIC engine_application)

# Copy all the dependencies ( dll ) to the executable file directory
add_custom_command(TARGET ProjectTactics POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:ProjectTactics> $<TARGET_RUNTIME_DLLS:ProjectTactics>
  COMMAND_EXPAND_LISTS
)

unset(SOURCES)