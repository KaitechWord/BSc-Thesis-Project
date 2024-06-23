########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

set(xkeyboard-config_COMPONENT_NAMES "")
if(DEFINED xkeyboard-config_FIND_DEPENDENCY_NAMES)
  list(APPEND xkeyboard-config_FIND_DEPENDENCY_NAMES )
  list(REMOVE_DUPLICATES xkeyboard-config_FIND_DEPENDENCY_NAMES)
else()
  set(xkeyboard-config_FIND_DEPENDENCY_NAMES )
endif()

########### VARIABLES #######################################################################
#############################################################################################
set(xkeyboard-config_PACKAGE_FOLDER_DEBUG "/home/kajtekk/.conan2/p/xkeybe449c83a142d6/p")
set(xkeyboard-config_BUILD_MODULES_PATHS_DEBUG )


set(xkeyboard-config_INCLUDE_DIRS_DEBUG )
set(xkeyboard-config_RES_DIRS_DEBUG )
set(xkeyboard-config_DEFINITIONS_DEBUG )
set(xkeyboard-config_SHARED_LINK_FLAGS_DEBUG )
set(xkeyboard-config_EXE_LINK_FLAGS_DEBUG )
set(xkeyboard-config_OBJECTS_DEBUG )
set(xkeyboard-config_COMPILE_DEFINITIONS_DEBUG )
set(xkeyboard-config_COMPILE_OPTIONS_C_DEBUG )
set(xkeyboard-config_COMPILE_OPTIONS_CXX_DEBUG )
set(xkeyboard-config_LIB_DIRS_DEBUG )
set(xkeyboard-config_BIN_DIRS_DEBUG "${xkeyboard-config_PACKAGE_FOLDER_DEBUG}/bin")
set(xkeyboard-config_LIBRARY_TYPE_DEBUG UNKNOWN)
set(xkeyboard-config_IS_HOST_WINDOWS_DEBUG 0)
set(xkeyboard-config_LIBS_DEBUG )
set(xkeyboard-config_SYSTEM_LIBS_DEBUG )
set(xkeyboard-config_FRAMEWORK_DIRS_DEBUG )
set(xkeyboard-config_FRAMEWORKS_DEBUG )
set(xkeyboard-config_BUILD_DIRS_DEBUG )
set(xkeyboard-config_NO_SONAME_MODE_DEBUG FALSE)


# COMPOUND VARIABLES
set(xkeyboard-config_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${xkeyboard-config_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${xkeyboard-config_COMPILE_OPTIONS_C_DEBUG}>")
set(xkeyboard-config_LINKER_FLAGS_DEBUG
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${xkeyboard-config_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${xkeyboard-config_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${xkeyboard-config_EXE_LINK_FLAGS_DEBUG}>")


set(xkeyboard-config_COMPONENTS_DEBUG )