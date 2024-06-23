########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

set(vdpau_COMPONENT_NAMES "")
if(DEFINED vdpau_FIND_DEPENDENCY_NAMES)
  list(APPEND vdpau_FIND_DEPENDENCY_NAMES )
  list(REMOVE_DUPLICATES vdpau_FIND_DEPENDENCY_NAMES)
else()
  set(vdpau_FIND_DEPENDENCY_NAMES )
endif()

########### VARIABLES #######################################################################
#############################################################################################
set(vdpau_PACKAGE_FOLDER_DEBUG "/home/kajtekk/.conan2/p/vdpau943d24b8df380/p")
set(vdpau_BUILD_MODULES_PATHS_DEBUG )


set(vdpau_INCLUDE_DIRS_DEBUG )
set(vdpau_RES_DIRS_DEBUG )
set(vdpau_DEFINITIONS_DEBUG )
set(vdpau_SHARED_LINK_FLAGS_DEBUG )
set(vdpau_EXE_LINK_FLAGS_DEBUG )
set(vdpau_OBJECTS_DEBUG )
set(vdpau_COMPILE_DEFINITIONS_DEBUG )
set(vdpau_COMPILE_OPTIONS_C_DEBUG )
set(vdpau_COMPILE_OPTIONS_CXX_DEBUG )
set(vdpau_LIB_DIRS_DEBUG )
set(vdpau_BIN_DIRS_DEBUG "${vdpau_PACKAGE_FOLDER_DEBUG}/bin")
set(vdpau_LIBRARY_TYPE_DEBUG SHARED)
set(vdpau_IS_HOST_WINDOWS_DEBUG 0)
set(vdpau_LIBS_DEBUG )
set(vdpau_SYSTEM_LIBS_DEBUG vdpau)
set(vdpau_FRAMEWORK_DIRS_DEBUG )
set(vdpau_FRAMEWORKS_DEBUG )
set(vdpau_BUILD_DIRS_DEBUG )
set(vdpau_NO_SONAME_MODE_DEBUG FALSE)


# COMPOUND VARIABLES
set(vdpau_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${vdpau_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${vdpau_COMPILE_OPTIONS_C_DEBUG}>")
set(vdpau_LINKER_FLAGS_DEBUG
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${vdpau_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${vdpau_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${vdpau_EXE_LINK_FLAGS_DEBUG}>")


set(vdpau_COMPONENTS_DEBUG )