# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
  imgui_canvas.cpp
  imgui_node_editor.cpp
  imgui_node_editor_api.cpp
  crude_json.cpp
  )

set(HEADERS
  crude_json.h
  imgui_bezier_math.h
  imgui_canvas.h
  imgui_bezier_math.h
  imgui_extra_math.h
  imgui_node_editor.h
  imgui_node_editor_internal.h
  imgui_node_editor_internal.h
  )
add_library(imgui_node_editor STATIC ${SOURCES} ${HEADERS})
target_link_libraries(imgui_node_editor PRIVATE imgui)
target_include_directories(imgui_node_editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# disable warning as error for imgui_node_editor
target_compile_options(imgui_node_editor PRIVATE /wd4701)
target_compile_options(imgui_node_editor PRIVATE /wd4996)

unset(SOURCES)
unset(HEADERS)