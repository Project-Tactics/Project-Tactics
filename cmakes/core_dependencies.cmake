macro(core_dependencies)
    # SDL2 dependency by configuring a wrapper library for simplified linking with other targets
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/prebuilt/sdl2")
    find_package(SDL2 CONFIG REQUIRED)
    # Create a wrapper library for SDL2
    add_library(sdl2_wrapper INTERFACE)
    # Link the wrapper library with SDL2 and SDL2main
    target_link_libraries(sdl2_wrapper INTERFACE SDL2::SDL2 SDL2::SDL2main)

    # OpenGL
    find_package(OpenGL REQUIRED)

    # assimp
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/prebuilt/assimp/cmake")
    find_package(assimp CONFIG REQUIRED)
    # Create a wrapper library for assimp
    add_library(assimp_wrapper INTERFACE)
    target_link_libraries(assimp_wrapper INTERFACE assimp::assimp ${ASSIMP_LIBRARIES})
    target_include_directories(assimp_wrapper INTERFACE ${ASSIMP_INCLUDE_DIRS})

    include(FetchContent)

    # nlhomann json
    FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
    FetchContent_MakeAvailable(json)
    add_library(json_wrapper INTERFACE)
    target_link_libraries(json_wrapper INTERFACE nlohmann_json::nlohmann_json)

    # jolt physics library
    FetchContent_Declare(
        JoltPhysics
        GIT_REPOSITORY "https://github.com/jrouwe/JoltPhysics"
        GIT_TAG "v5.3.0"
		SOURCE_SUBDIR "Build")
    FetchContent_MakeAvailable(JoltPhysics)
    set_property(TARGET Jolt PROPERTY FOLDER "thirdparty")

    # gtest
    FetchContent_Declare(googletest 
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    set_property(TARGET gmock gmock_main gtest gtest_main PROPERTY FOLDER "thirdparty")
    
    # cpptrace
    FetchContent_Declare(cpptrace
        GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
        GIT_TAG v0.6.2)
    FetchContent_MakeAvailable(cpptrace)
    set_property(TARGET cpptrace-lib PROPERTY FOLDER "thirdparty")
endmacro()
