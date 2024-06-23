########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

list(APPEND libcap_COMPONENT_NAMES libcap::cap)
list(REMOVE_DUPLICATES libcap_COMPONENT_NAMES)
if(DEFINED libcap_FIND_DEPENDENCY_NAMES)
  list(APPEND libcap_FIND_DEPENDENCY_NAMES )
  list(REMOVE_DUPLICATES libcap_FIND_DEPENDENCY_NAMES)
else()
  set(libcap_FIND_DEPENDENCY_NAMES )
endif()

########### VARIABLES #######################################################################
#############################################################################################
set(libcap_PACKAGE_FOLDER_DEBUG "/home/kajtekk/.conan2/p/b/libca025171a8669ee/p")
set(libcap_BUILD_MODULES_PATHS_DEBUG )


set(libcap_INCLUDE_DIRS_DEBUG )
set(libcap_RES_DIRS_DEBUG )
set(libcap_DEFINITIONS_DEBUG )
set(libcap_SHARED_LINK_FLAGS_DEBUG )
set(libcap_EXE_LINK_FLAGS_DEBUG )
set(libcap_OBJECTS_DEBUG )
set(libcap_COMPILE_DEFINITIONS_DEBUG )
set(libcap_COMPILE_OPTIONS_C_DEBUG )
set(libcap_COMPILE_OPTIONS_CXX_DEBUG )
set(libcap_LIB_DIRS_DEBUG "${libcap_PACKAGE_FOLDER_DEBUG}/lib")
set(libcap_BIN_DIRS_DEBUG )
set(libcap_LIBRARY_TYPE_DEBUG STATIC)
set(libcap_IS_HOST_WINDOWS_DEBUG 0)
set(libcap_LIBS_DEBUG cap)
set(libcap_SYSTEM_LIBS_DEBUG )
set(libcap_FRAMEWORK_DIRS_DEBUG )
set(libcap_FRAMEWORKS_DEBUG )
set(libcap_BUILD_DIRS_DEBUG )
set(libcap_NO_SONAME_MODE_DEBUG FALSE)


# COMPOUND VARIABLES
set(libcap_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${libcap_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${libcap_COMPILE_OPTIONS_C_DEBUG}>")
set(libcap_LINKER_FLAGS_DEBUG
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${libcap_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${libcap_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${libcap_EXE_LINK_FLAGS_DEBUG}>")


set(libcap_COMPONENTS_DEBUG libcap::cap)
########### COMPONENT libcap::cap VARIABLES ############################################

set(libcap_libcap_cap_INCLUDE_DIRS_DEBUG )
set(libcap_libcap_cap_LIB_DIRS_DEBUG "${libcap_PACKAGE_FOLDER_DEBUG}/lib")
set(libcap_libcap_cap_BIN_DIRS_DEBUG )
set(libcap_libcap_cap_LIBRARY_TYPE_DEBUG STATIC)
set(libcap_libcap_cap_IS_HOST_WINDOWS_DEBUG 0)
set(libcap_libcap_cap_RES_DIRS_DEBUG )
set(libcap_libcap_cap_DEFINITIONS_DEBUG )
set(libcap_libcap_cap_OBJECTS_DEBUG )
set(libcap_libcap_cap_COMPILE_DEFINITIONS_DEBUG )
set(libcap_libcap_cap_COMPILE_OPTIONS_C_DEBUG "")
set(libcap_libcap_cap_COMPILE_OPTIONS_CXX_DEBUG "")
set(libcap_libcap_cap_LIBS_DEBUG cap)
set(libcap_libcap_cap_SYSTEM_LIBS_DEBUG )
set(libcap_libcap_cap_FRAMEWORK_DIRS_DEBUG )
set(libcap_libcap_cap_FRAMEWORKS_DEBUG )
set(libcap_libcap_cap_DEPENDENCIES_DEBUG )
set(libcap_libcap_cap_SHARED_LINK_FLAGS_DEBUG )
set(libcap_libcap_cap_EXE_LINK_FLAGS_DEBUG )
set(libcap_libcap_cap_NO_SONAME_MODE_DEBUG FALSE)

# COMPOUND VARIABLES
set(libcap_libcap_cap_LINKER_FLAGS_DEBUG
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${libcap_libcap_cap_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${libcap_libcap_cap_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${libcap_libcap_cap_EXE_LINK_FLAGS_DEBUG}>
)
set(libcap_libcap_cap_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${libcap_libcap_cap_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${libcap_libcap_cap_COMPILE_OPTIONS_C_DEBUG}>")