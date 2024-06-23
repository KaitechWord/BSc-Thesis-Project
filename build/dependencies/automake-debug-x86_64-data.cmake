########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

set(automake_COMPONENT_NAMES "")
if(DEFINED automake_FIND_DEPENDENCY_NAMES)
  list(APPEND automake_FIND_DEPENDENCY_NAMES autoconf)
  list(REMOVE_DUPLICATES automake_FIND_DEPENDENCY_NAMES)
else()
  set(automake_FIND_DEPENDENCY_NAMES autoconf)
endif()
set(autoconf_FIND_MODE "NO_MODULE")

########### VARIABLES #######################################################################
#############################################################################################
set(automake_PACKAGE_FOLDER_DEBUG "/home/kajtekk/.conan2/p/autom480a421c82a75/p")
set(automake_BUILD_MODULES_PATHS_DEBUG )


set(automake_INCLUDE_DIRS_DEBUG )
set(automake_RES_DIRS_DEBUG "${automake_PACKAGE_FOLDER_DEBUG}/res")
set(automake_DEFINITIONS_DEBUG )
set(automake_SHARED_LINK_FLAGS_DEBUG )
set(automake_EXE_LINK_FLAGS_DEBUG )
set(automake_OBJECTS_DEBUG )
set(automake_COMPILE_DEFINITIONS_DEBUG )
set(automake_COMPILE_OPTIONS_C_DEBUG )
set(automake_COMPILE_OPTIONS_CXX_DEBUG )
set(automake_LIB_DIRS_DEBUG )
set(automake_BIN_DIRS_DEBUG "${automake_PACKAGE_FOLDER_DEBUG}/bin")
set(automake_LIBRARY_TYPE_DEBUG UNKNOWN)
set(automake_IS_HOST_WINDOWS_DEBUG 0)
set(automake_LIBS_DEBUG )
set(automake_SYSTEM_LIBS_DEBUG )
set(automake_FRAMEWORK_DIRS_DEBUG )
set(automake_FRAMEWORKS_DEBUG )
set(automake_BUILD_DIRS_DEBUG )
set(automake_NO_SONAME_MODE_DEBUG FALSE)


# COMPOUND VARIABLES
set(automake_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${automake_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${automake_COMPILE_OPTIONS_C_DEBUG}>")
set(automake_LINKER_FLAGS_DEBUG
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${automake_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${automake_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${automake_EXE_LINK_FLAGS_DEBUG}>")


set(automake_COMPONENTS_DEBUG )