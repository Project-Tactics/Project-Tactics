macro(add_to_engine_folder TARGET_NAME)
    add_to_folder(${TARGET_NAME} "tactics/engine")
endmacro()

macro(add_to_libs_folder TARGET_NAME)
    add_to_folder(${TARGET_NAME} "tactics/libs")
endmacro()

macro(add_to_data_folder TARGET_NAME)
    add_to_folder(${TARGET_NAME} "apps/data")
endmacro()

macro(add_to_thirdparty_folder TARGET_NAME)
    add_to_folder(${TARGET_NAME} "thirdparty")
endmacro()

macro(add_to_apps_folder TARGET_NAME)
    add_to_folder(${TARGET_NAME} "apps")
endmacro()

macro(add_to_folder TARGET_NAME FOLDER_NAME)
    # TODO(Gerarky) it's not ideal to guess the source files like this. They should be passed in as arguments
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCES} ${HEADERS} ${TEST_SOURCES} ${FILES})
    source_group("_dev" FILES CMakeLists.txt)
    set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER ${FOLDER_NAME})
endmacro()