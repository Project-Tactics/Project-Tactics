cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
    EventsSystem.cpp
    EventsListener.cpp)

set(HEADERS
    EventsSystem.h
    EventsListener.h)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS})

add_library(libs_events STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_events PUBLIC sdl2_wrapper)
add_to_libs_folder(libs_events)

unset(SOURCES)
unset(HEADERS)
