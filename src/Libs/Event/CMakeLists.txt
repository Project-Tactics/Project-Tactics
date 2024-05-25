cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    EventsSystem.cpp
    EventsListener.cpp)

set(HEADERS
    EventsSystem.h
    EventsListener.h)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(libs_event STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_event PUBLIC sdl2_wrapper PRIVATE imgui)
add_to_libs_folder(libs_event)

unset(SOURCES)
unset(HEADERS)
