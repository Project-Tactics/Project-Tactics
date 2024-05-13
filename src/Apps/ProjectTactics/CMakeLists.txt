# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(APP_PROJECTTACTICS_SOURCES
        ProjectTacticsMain.cpp)

add_executable(ProjectTactics ${APP_PROJECTTACTICS_SOURCES})
target_link_libraries(ProjectTactics PUBLIC sdl2_wrapper glad)

# Copy all the dependencies ( dll ) to the executable file directory
add_custom_command(TARGET ProjectTactics POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:ProjectTactics> $<TARGET_RUNTIME_DLLS:ProjectTactics>
  COMMAND_EXPAND_LISTS
)
