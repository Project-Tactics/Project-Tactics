macro(core_dependencies)
    # Set up SDL2 dependency by configuring a wrapper library for simplified linking with other targets
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}"/deps/sdl2/cmake")
    find_package(SDL2 CONFIG REQUIRED)
    # Create a wrapper library for SDL2
    add_library(sdl2_wrapper INTERFACE)
    # Link the wrapper library with SDL2 and SDL2main
    target_link_libraries(sdl2_wrapper INTERFACE SDL2::SDL2 SDL2::SDL2main)

    # Set up OpenGL
    find_package(OpenGL REQUIRED)

    # Set up Lua
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}"/deps/lua/cmake")
    find_package(LUA CONFIG REQUIRED)
    # Create a wrapper library for lua
    add_library(lua_wrapper INTERFACE)
    # Link the wrapper library with lua51
    target_link_libraries(lua_wrapper INTERFACE lua::lua)
endmacro()
