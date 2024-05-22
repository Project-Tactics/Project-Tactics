# Locate Assimp library (4.x)
#
# This module defines:
#
# ASSIMP_FOUND, if false, do not try to link to Assimp
# ASSIMP_LIBRARY, the name of the library to link against
# ASSIMP_LIBRARIES, the full list of libs to link against
# ASSIMP_INCLUDE_DIR, where to find assimp/*.h files
#=============================================================================

include(ExternalProject)
include(FindPackageHandleStandardArgs)

# Setup configuration for assimp
set(ASSIMP_CMAKE_ARGS
  -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
  -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
  -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}
  -DBUILD_SHARED_LIBS:BOOL=OFF
  -DASSIMP_NO_EXPORT:BOOL=ON
  -DASSIMP_BUILD_ASSIMP_TOOLS:BOOL=OFF
  -DASSIMP_BUILD_TESTS:BOOL=OFF
  -DASSIMP_BUILD_ZLIB:BOOL=ON
  -DASSIMP_INSTALL_PDB:BOOL=OFF
)

ExternalProject_Add(external_assimp
  URL https://github.com/assimp/assimp/archive/v5.4.1.zip
  URL_MD5 0354d56d4b10a593f205d24adb5f1c5b
  CMAKE_ARGS ${ASSIMP_CMAKE_ARGS}
)
message("${install_dir}")
ExternalProject_Get_Property(external_assimp install_dir)
message("${install_dir}")

set(ASSIMP_INCLUDE_DIR ${install_dir}/include)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(LIB_POSTFIX -mtd)
else()
    set(LIB_POSTFIX -mt)
endif()

set(ASSIMP_LIBRARY ${install_dir}/lib/assimp-vc143${LIB_POSTFIX}.lib)
set(IRRXML_LIBRARY ${install_dir}/lib/IrrXML.lib)
# TODO(Gerark) Maybe we should use the non-debug version when linking in release
set(ZLIB_LIBRARY ${install_dir}/lib/zlibstaticd.lib)

unset(LIB_POSTFIX)

# Create Library
add_library(assimp::assimp STATIC IMPORTED)
set_property(TARGET assimp::assimp PROPERTY IMPORTED_LOCATION ${ASSIMP_LIBRARY})
add_dependencies(assimp::assimp external_assimp)

find_package_handle_standard_args(assimp
  DEFAULT_MSG ASSIMP_INCLUDE_DIR
  ASSIMP_LIBRARY
)

if (ASSIMP_FOUND)
	set(ASSIMP_LIBRARIES ${ASSIMP_LIBRARY} ${IRRXML_LIBRARY} ${ZLIB_LIBRARY})
	set(ASSIMP_INCLUDE_DIRS ${ASSIMP_INCLUDE_DIR})
endif ()
