########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

set(libtool_COMPONENT_NAMES "")
if(DEFINED libtool_FIND_DEPENDENCY_NAMES)
  list(APPEND libtool_FIND_DEPENDENCY_NAMES automake)
  list(REMOVE_DUPLICATES libtool_FIND_DEPENDENCY_NAMES)
else()
  set(libtool_FIND_DEPENDENCY_NAMES automake)
endif()
set(automake_FIND_MODE "NO_MODULE")

########### VARIABLES #######################################################################
#############################################################################################
set(libtool_PACKAGE_FOLDER_DEBUG "/home/kajtekk/.conan2/p/b/libto7c8b19ede4173/p")
set(libtool_BUILD_MODULES_PATHS_DEBUG )


set(libtool_INCLUDE_DIRS_DEBUG )
set(libtool_RES_DIRS_DEBUG )
set(libtool_DEFINITIONS_DEBUG )
set(libtool_SHARED_LINK_FLAGS_DEBUG )
set(libtool_EXE_LINK_FLAGS_DEBUG )
set(libtool_OBJECTS_DEBUG )
set(libtool_COMPILE_DEFINITIONS_DEBUG )
set(libtool_COMPILE_OPTIONS_C_DEBUG )
set(libtool_COMPILE_OPTIONS_CXX_DEBUG )
set(libtool_LIB_DIRS_DEBUG "${libtool_PACKAGE_FOLDER_DEBUG}/lib")
set(libtool_BIN_DIRS_DEBUG )
set(libtool_LIBRARY_TYPE_DEBUG STATIC)
set(libtool_IS_HOST_WINDOWS_DEBUG 0)
set(libtool_LIBS_DEBUG ltdl)
set(libtool_SYSTEM_LIBS_DEBUG dl)
set(libtool_FRAMEWORK_DIRS_DEBUG )
set(libtool_FRAMEWORKS_DEBUG )
set(libtool_BUILD_DIRS_DEBUG )
set(libtool_NO_SONAME_MODE_DEBUG FALSE)


# COMPOUND VARIABLES
set(libtool_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${libtool_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${libtool_COMPILE_OPTIONS_C_DEBUG}>")
set(libtool_LINKER_FLAGS_DEBUG
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${libtool_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${libtool_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${libtool_EXE_LINK_FLAGS_DEBUG}>")


set(libtool_COMPONENTS_DEBUG )