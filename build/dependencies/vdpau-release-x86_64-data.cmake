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
set(vdpau_PACKAGE_FOLDER_RELEASE "/home/kajtekk/.conan2/p/vdpau943d24b8df380/p")
set(vdpau_BUILD_MODULES_PATHS_RELEASE )


set(vdpau_INCLUDE_DIRS_RELEASE )
set(vdpau_RES_DIRS_RELEASE )
set(vdpau_DEFINITIONS_RELEASE )
set(vdpau_SHARED_LINK_FLAGS_RELEASE )
set(vdpau_EXE_LINK_FLAGS_RELEASE )
set(vdpau_OBJECTS_RELEASE )
set(vdpau_COMPILE_DEFINITIONS_RELEASE )
set(vdpau_COMPILE_OPTIONS_C_RELEASE )
set(vdpau_COMPILE_OPTIONS_CXX_RELEASE )
set(vdpau_LIB_DIRS_RELEASE )
set(vdpau_BIN_DIRS_RELEASE "${vdpau_PACKAGE_FOLDER_RELEASE}/bin")
set(vdpau_LIBRARY_TYPE_RELEASE SHARED)
set(vdpau_IS_HOST_WINDOWS_RELEASE 0)
set(vdpau_LIBS_RELEASE )
set(vdpau_SYSTEM_LIBS_RELEASE vdpau)
set(vdpau_FRAMEWORK_DIRS_RELEASE )
set(vdpau_FRAMEWORKS_RELEASE )
set(vdpau_BUILD_DIRS_RELEASE )
set(vdpau_NO_SONAME_MODE_RELEASE FALSE)


# COMPOUND VARIABLES
set(vdpau_COMPILE_OPTIONS_RELEASE
    "$<$<COMPILE_LANGUAGE:CXX>:${vdpau_COMPILE_OPTIONS_CXX_RELEASE}>"
    "$<$<COMPILE_LANGUAGE:C>:${vdpau_COMPILE_OPTIONS_C_RELEASE}>")
set(vdpau_LINKER_FLAGS_RELEASE
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${vdpau_SHARED_LINK_FLAGS_RELEASE}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${vdpau_SHARED_LINK_FLAGS_RELEASE}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${vdpau_EXE_LINK_FLAGS_RELEASE}>")


set(vdpau_COMPONENTS_RELEASE )