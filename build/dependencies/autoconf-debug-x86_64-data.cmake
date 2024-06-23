########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

set(autoconf_COMPONENT_NAMES "")
if(DEFINED autoconf_FIND_DEPENDENCY_NAMES)
  list(APPEND autoconf_FIND_DEPENDENCY_NAMES m4)
  list(REMOVE_DUPLICATES autoconf_FIND_DEPENDENCY_NAMES)
else()
  set(autoconf_FIND_DEPENDENCY_NAMES m4)
endif()
set(m4_FIND_MODE "NO_MODULE")

########### VARIABLES #######################################################################
#############################################################################################
set(autoconf_PACKAGE_FOLDER_DEBUG "/home/kajtekk/.conan2/p/autocf2af015330354/p")
set(autoconf_BUILD_MODULES_PATHS_DEBUG )


set(autoconf_INCLUDE_DIRS_DEBUG )
set(autoconf_RES_DIRS_DEBUG "${autoconf_PACKAGE_FOLDER_DEBUG}/res")
set(autoconf_DEFINITIONS_DEBUG )
set(autoconf_SHARED_LINK_FLAGS_DEBUG )
set(autoconf_EXE_LINK_FLAGS_DEBUG )
set(autoconf_OBJECTS_DEBUG )
set(autoconf_COMPILE_DEFINITIONS_DEBUG )
set(autoconf_COMPILE_OPTIONS_C_DEBUG )
set(autoconf_COMPILE_OPTIONS_CXX_DEBUG )
set(autoconf_LIB_DIRS_DEBUG )
set(autoconf_BIN_DIRS_DEBUG "${autoconf_PACKAGE_FOLDER_DEBUG}/bin")
set(autoconf_LIBRARY_TYPE_DEBUG UNKNOWN)
set(autoconf_IS_HOST_WINDOWS_DEBUG 0)
set(autoconf_LIBS_DEBUG )
set(autoconf_SYSTEM_LIBS_DEBUG )
set(autoconf_FRAMEWORK_DIRS_DEBUG )
set(autoconf_FRAMEWORKS_DEBUG )
set(autoconf_BUILD_DIRS_DEBUG )
set(autoconf_NO_SONAME_MODE_DEBUG FALSE)


# COMPOUND VARIABLES
set(autoconf_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${autoconf_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${autoconf_COMPILE_OPTIONS_C_DEBUG}>")
set(autoconf_LINKER_FLAGS_DEBUG
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${autoconf_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${autoconf_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${autoconf_EXE_LINK_FLAGS_DEBUG}>")


set(autoconf_COMPONENTS_DEBUG )