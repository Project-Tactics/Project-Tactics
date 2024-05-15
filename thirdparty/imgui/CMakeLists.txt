# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
  imgui/imgui_draw.cpp
  imgui/imgui.cpp
  imgui/imgui_demo.cpp
  imgui/imgui_draw.cpp
  imgui/imgui_tables.cpp
  imgui/imgui_widgets.cpp
  imgui/backends/imgui_impl_opengl3.cpp
  imgui/backends/imgui_impl_sdl2.cpp
  )

set(HEADERS
  imgui/imconfig.h
  imgui/imgui.h
  imgui/imgui_internal.h
  imgui/imstb_rectpack.h
  imgui/imstb_textedit.h
  imgui/imstb_truetype.h
  imgui/backends/imgui_impl_opengl3.h
  imgui/backends/imgui_impl_sdl2.h
  )
add_library(imgui STATIC ${SOURCES} ${HEADERS})
target_link_libraries(imgui PRIVATE sdl2_wrapper)
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
# disable warning as error for imgui
target_compile_options(imgui PRIVATE /wd4701)

unset(SOURCES)
unset(HEADERS)