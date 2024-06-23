########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

set(libx265_COMPONENT_NAMES "")
if(DEFINED libx265_FIND_DEPENDENCY_NAMES)
  list(APPEND libx265_FIND_DEPENDENCY_NAMES )
  list(REMOVE_DUPLICATES libx265_FIND_DEPENDENCY_NAMES)
else()
  set(libx265_FIND_DEPENDENCY_NAMES )
endif()

########### VARIABLES #######################################################################
#############################################################################################
set(libx265_PACKAGE_FOLDER_DEBUG "/home/kajtekk/.conan2/p/b/libx26c562c20c3bf6/p")
set(libx265_BUILD_MODULES_PATHS_DEBUG )


set(libx265_INCLUDE_DIRS_DEBUG )
set(libx265_RES_DIRS_DEBUG )
set(libx265_DEFINITIONS_DEBUG )
set(libx265_SHARED_LINK_FLAGS_DEBUG "-Wl,-Bsymbolic,-znoexecstack")
set(libx265_EXE_LINK_FLAGS_DEBUG )
set(libx265_OBJECTS_DEBUG )
set(libx265_COMPILE_DEFINITIONS_DEBUG )
set(libx265_COMPILE_OPTIONS_C_DEBUG )
set(libx265_COMPILE_OPTIONS_CXX_DEBUG )
set(libx265_LIB_DIRS_DEBUG "${libx265_PACKAGE_FOLDER_DEBUG}/lib")
set(libx265_BIN_DIRS_DEBUG )
set(libx265_LIBRARY_TYPE_DEBUG STATIC)
set(libx265_IS_HOST_WINDOWS_DEBUG 0)
set(libx265_LIBS_DEBUG x265)
set(libx265_SYSTEM_LIBS_DEBUG dl pthread m rt stdc++)
set(libx265_FRAMEWORK_DIRS_DEBUG )
set(libx265_FRAMEWORKS_DEBUG )
set(libx265_BUILD_DIRS_DEBUG )
set(libx265_NO_SONAME_MODE_DEBUG FALSE)


# COMPOUND VARIABLES
set(libx265_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${libx265_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${libx265_COMPILE_OPTIONS_C_DEBUG}>")
set(libx265_LINKER_FLAGS_DEBUG
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${libx265_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${libx265_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${libx265_EXE_LINK_FLAGS_DEBUG}>")


set(libx265_COMPONENTS_DEBUG )