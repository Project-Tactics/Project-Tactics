macro(add_to_engine_folder TARGET_NAME)
    set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "tactics/engine")
endmacro()

macro(add_to_libs_folder TARGET_NAME)
    set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "tactics/libs")
endmacro()

macro(add_to_data_folder TARGET_NAME)
    set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "tactics/data")
endmacro()

macro(add_to_thirdparty_folder TARGET_NAME)
    set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "thirdparty")
endmacro()