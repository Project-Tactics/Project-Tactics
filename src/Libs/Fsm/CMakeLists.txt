cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(SOURCES
        Fsm.cpp
        FsmAction.cpp
        FsmBuilder.cpp)

set(HEADERS
        Fsm.h
        FsmAction.h
        FsmBuilder.h
        FsmExternalController.h
        FsmState.h
        FsmStateWithServices.h
        FsmTypes.h)

add_library(libs_fsm STATIC ${SOURCES} ${HEADERS})
target_link_libraries(libs_fsm PUBLIC libs_utility)
add_to_libs_folder(libs_fsm)

set(TEST_SOURCES Tests/Test_Fsm.cpp)
set(TEST_LIBS libs_fsm)
add_unit_test(libs_fsm ${TEST_SOURCES} ${TEST_LIBS})

unset(SOURCES)
unset(HEADERS)
unset(TEST_SOURCES)