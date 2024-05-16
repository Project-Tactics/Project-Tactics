# Ensure the minimum required version of CMake is available, otherwise, throw an error
cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

# Copied from `configure_package_config_file`
macro(set_and_check _var _file)
    set(${_var} "${_file}")
    if(NOT EXISTS "${_file}")
        message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
    endif()
endmacro()

# Copied from `configure_package_config_file`
macro(check_required_components _NAME)
    foreach(comp ${${_NAME}_FIND_COMPONENTS})
        if(NOT ${_NAME}_${comp}_FOUND)
            if(${_NAME}_FIND_REQUIRED_${comp})
                set(${_NAME}_FOUND FALSE)
            endif()
        endif()
    endforeach()
endmacro()

set(LUA_FOUND TRUE)

if(CMAKE_SIZEOF_VOID_P STREQUAL "4")
    set(_lua_arch_subdir "x86")
elseif(CMAKE_SIZEOF_VOID_P STREQUAL "8")
    set(_lua_arch_subdir "x64")
else()
    set(LUA_FOUND FALSE)
    return()
endif()

set_and_check(LUA_PREFIX       "${CMAKE_CURRENT_LIST_DIR}/..")
set_and_check(LUA_INCLUDE_DIR  "${LUA_PREFIX}/include")
set(LUA_INCLUDE_DIRS           "${LUA_INCLUDE_DIR}")
set_and_check(LUA_LIBDIR       "${LUA_PREFIX}/lib/${_sdl_arch_subdir}")

set_and_check(_lua_library     "${LUA_LIBDIR}/lua51.lib")
if(EXISTS "${_lua_library}")
    if(NOT TARGET lua::lua)
        add_library(lua::lua STATIC IMPORTED)
        set_target_properties(lua::lua
            PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${LUA_INCLUDE_DIRS}"
                IMPORTED_LOCATION "${_lua_library}"
        )
    endif()
    set(LUA_LUA_FOUND TRUE)
else()
    set(LUA_LUA_FOUND FALSE)
endif()
unset(_lua_library)

check_required_components(lua)
