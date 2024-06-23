########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

list(APPEND vaapi_COMPONENT_NAMES vaapi::libva vaapi::libva-x11 vaapi::libva-drm)
list(REMOVE_DUPLICATES vaapi_COMPONENT_NAMES)
if(DEFINED vaapi_FIND_DEPENDENCY_NAMES)
  list(APPEND vaapi_FIND_DEPENDENCY_NAMES )
  list(REMOVE_DUPLICATES vaapi_FIND_DEPENDENCY_NAMES)
else()
  set(vaapi_FIND_DEPENDENCY_NAMES )
endif()

########### VARIABLES #######################################################################
#############################################################################################
set(vaapi_PACKAGE_FOLDER_RELEASE "/home/kajtekk/.conan2/p/vaapiee72fb83987e1/p")
set(vaapi_BUILD_MODULES_PATHS_RELEASE )


set(vaapi_INCLUDE_DIRS_RELEASE )
set(vaapi_RES_DIRS_RELEASE )
set(vaapi_DEFINITIONS_RELEASE )
set(vaapi_SHARED_LINK_FLAGS_RELEASE )
set(vaapi_EXE_LINK_FLAGS_RELEASE )
set(vaapi_OBJECTS_RELEASE )
set(vaapi_COMPILE_DEFINITIONS_RELEASE )
set(vaapi_COMPILE_OPTIONS_C_RELEASE )
set(vaapi_COMPILE_OPTIONS_CXX_RELEASE )
set(vaapi_LIB_DIRS_RELEASE )
set(vaapi_BIN_DIRS_RELEASE "${vaapi_PACKAGE_FOLDER_RELEASE}/bin")
set(vaapi_LIBRARY_TYPE_RELEASE SHARED)
set(vaapi_IS_HOST_WINDOWS_RELEASE 0)
set(vaapi_LIBS_RELEASE )
set(vaapi_SYSTEM_LIBS_RELEASE va-drm va va-x11)
set(vaapi_FRAMEWORK_DIRS_RELEASE )
set(vaapi_FRAMEWORKS_RELEASE )
set(vaapi_BUILD_DIRS_RELEASE )
set(vaapi_NO_SONAME_MODE_RELEASE FALSE)


# COMPOUND VARIABLES
set(vaapi_COMPILE_OPTIONS_RELEASE
    "$<$<COMPILE_LANGUAGE:CXX>:${vaapi_COMPILE_OPTIONS_CXX_RELEASE}>"
    "$<$<COMPILE_LANGUAGE:C>:${vaapi_COMPILE_OPTIONS_C_RELEASE}>")
set(vaapi_LINKER_FLAGS_RELEASE
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${vaapi_SHARED_LINK_FLAGS_RELEASE}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${vaapi_SHARED_LINK_FLAGS_RELEASE}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${vaapi_EXE_LINK_FLAGS_RELEASE}>")


set(vaapi_COMPONENTS_RELEASE vaapi::libva vaapi::libva-x11 vaapi::libva-drm)
########### COMPONENT vaapi::libva-drm VARIABLES ############################################

set(vaapi_vaapi_libva-drm_INCLUDE_DIRS_RELEASE )
set(vaapi_vaapi_libva-drm_LIB_DIRS_RELEASE )
set(vaapi_vaapi_libva-drm_BIN_DIRS_RELEASE "${vaapi_PACKAGE_FOLDER_RELEASE}/bin")
set(vaapi_vaapi_libva-drm_LIBRARY_TYPE_RELEASE SHARED)
set(vaapi_vaapi_libva-drm_IS_HOST_WINDOWS_RELEASE 0)
set(vaapi_vaapi_libva-drm_RES_DIRS_RELEASE )
set(vaapi_vaapi_libva-drm_DEFINITIONS_RELEASE )
set(vaapi_vaapi_libva-drm_OBJECTS_RELEASE )
set(vaapi_vaapi_libva-drm_COMPILE_DEFINITIONS_RELEASE )
set(vaapi_vaapi_libva-drm_COMPILE_OPTIONS_C_RELEASE "")
set(vaapi_vaapi_libva-drm_COMPILE_OPTIONS_CXX_RELEASE "")
set(vaapi_vaapi_libva-drm_LIBS_RELEASE )
set(vaapi_vaapi_libva-drm_SYSTEM_LIBS_RELEASE va-drm va)
set(vaapi_vaapi_libva-drm_FRAMEWORK_DIRS_RELEASE )
set(vaapi_vaapi_libva-drm_FRAMEWORKS_RELEASE )
set(vaapi_vaapi_libva-drm_DEPENDENCIES_RELEASE )
set(vaapi_vaapi_libva-drm_SHARED_LINK_FLAGS_RELEASE )
set(vaapi_vaapi_libva-drm_EXE_LINK_FLAGS_RELEASE )
set(vaapi_vaapi_libva-drm_NO_SONAME_MODE_RELEASE FALSE)

# COMPOUND VARIABLES
set(vaapi_vaapi_libva-drm_LINKER_FLAGS_RELEASE
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${vaapi_vaapi_libva-drm_SHARED_LINK_FLAGS_RELEASE}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${vaapi_vaapi_libva-drm_SHARED_LINK_FLAGS_RELEASE}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${vaapi_vaapi_libva-drm_EXE_LINK_FLAGS_RELEASE}>
)
set(vaapi_vaapi_libva-drm_COMPILE_OPTIONS_RELEASE
    "$<$<COMPILE_LANGUAGE:CXX>:${vaapi_vaapi_libva-drm_COMPILE_OPTIONS_CXX_RELEASE}>"
    "$<$<COMPILE_LANGUAGE:C>:${vaapi_vaapi_libva-drm_COMPILE_OPTIONS_C_RELEASE}>")
########### COMPONENT vaapi::libva-x11 VARIABLES ############################################

set(vaapi_vaapi_libva-x11_INCLUDE_DIRS_RELEASE )
set(vaapi_vaapi_libva-x11_LIB_DIRS_RELEASE )
set(vaapi_vaapi_libva-x11_BIN_DIRS_RELEASE "${vaapi_PACKAGE_FOLDER_RELEASE}/bin")
set(vaapi_vaapi_libva-x11_LIBRARY_TYPE_RELEASE SHARED)
set(vaapi_vaapi_libva-x11_IS_HOST_WINDOWS_RELEASE 0)
set(vaapi_vaapi_libva-x11_RES_DIRS_RELEASE )
set(vaapi_vaapi_libva-x11_DEFINITIONS_RELEASE )
set(vaapi_vaapi_libva-x11_OBJECTS_RELEASE )
set(vaapi_vaapi_libva-x11_COMPILE_DEFINITIONS_RELEASE )
set(vaapi_vaapi_libva-x11_COMPILE_OPTIONS_C_RELEASE "")
set(vaapi_vaapi_libva-x11_COMPILE_OPTIONS_CXX_RELEASE "")
set(vaapi_vaapi_libva-x11_LIBS_RELEASE )
set(vaapi_vaapi_libva-x11_SYSTEM_LIBS_RELEASE va-x11 va)
set(vaapi_vaapi_libva-x11_FRAMEWORK_DIRS_RELEASE )
set(vaapi_vaapi_libva-x11_FRAMEWORKS_RELEASE )
set(vaapi_vaapi_libva-x11_DEPENDENCIES_RELEASE )
set(vaapi_vaapi_libva-x11_SHARED_LINK_FLAGS_RELEASE )
set(vaapi_vaapi_libva-x11_EXE_LINK_FLAGS_RELEASE )
set(vaapi_vaapi_libva-x11_NO_SONAME_MODE_RELEASE FALSE)

# COMPOUND VARIABLES
set(vaapi_vaapi_libva-x11_LINKER_FLAGS_RELEASE
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${vaapi_vaapi_libva-x11_SHARED_LINK_FLAGS_RELEASE}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${vaapi_vaapi_libva-x11_SHARED_LINK_FLAGS_RELEASE}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${vaapi_vaapi_libva-x11_EXE_LINK_FLAGS_RELEASE}>
)
set(vaapi_vaapi_libva-x11_COMPILE_OPTIONS_RELEASE
    "$<$<COMPILE_LANGUAGE:CXX>:${vaapi_vaapi_libva-x11_COMPILE_OPTIONS_CXX_RELEASE}>"
    "$<$<COMPILE_LANGUAGE:C>:${vaapi_vaapi_libva-x11_COMPILE_OPTIONS_C_RELEASE}>")
########### COMPONENT vaapi::libva VARIABLES ############################################

set(vaapi_vaapi_libva_INCLUDE_DIRS_RELEASE )
set(vaapi_vaapi_libva_LIB_DIRS_RELEASE )
set(vaapi_vaapi_libva_BIN_DIRS_RELEASE "${vaapi_PACKAGE_FOLDER_RELEASE}/bin")
set(vaapi_vaapi_libva_LIBRARY_TYPE_RELEASE SHARED)
set(vaapi_vaapi_libva_IS_HOST_WINDOWS_RELEASE 0)
set(vaapi_vaapi_libva_RES_DIRS_RELEASE )
set(vaapi_vaapi_libva_DEFINITIONS_RELEASE )
set(vaapi_vaapi_libva_OBJECTS_RELEASE )
set(vaapi_vaapi_libva_COMPILE_DEFINITIONS_RELEASE )
set(vaapi_vaapi_libva_COMPILE_OPTIONS_C_RELEASE "")
set(vaapi_vaapi_libva_COMPILE_OPTIONS_CXX_RELEASE "")
set(vaapi_vaapi_libva_LIBS_RELEASE )
set(vaapi_vaapi_libva_SYSTEM_LIBS_RELEASE va)
set(vaapi_vaapi_libva_FRAMEWORK_DIRS_RELEASE )
set(vaapi_vaapi_libva_FRAMEWORKS_RELEASE )
set(vaapi_vaapi_libva_DEPENDENCIES_RELEASE )
set(vaapi_vaapi_libva_SHARED_LINK_FLAGS_RELEASE )
set(vaapi_vaapi_libva_EXE_LINK_FLAGS_RELEASE )
set(vaapi_vaapi_libva_NO_SONAME_MODE_RELEASE FALSE)

# COMPOUND VARIABLES
set(vaapi_vaapi_libva_LINKER_FLAGS_RELEASE
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${vaapi_vaapi_libva_SHARED_LINK_FLAGS_RELEASE}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${vaapi_vaapi_libva_SHARED_LINK_FLAGS_RELEASE}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${vaapi_vaapi_libva_EXE_LINK_FLAGS_RELEASE}>
)
set(vaapi_vaapi_libva_COMPILE_OPTIONS_RELEASE
    "$<$<COMPILE_LANGUAGE:CXX>:${vaapi_vaapi_libva_COMPILE_OPTIONS_CXX_RELEASE}>"
    "$<$<COMPILE_LANGUAGE:C>:${vaapi_vaapi_libva_COMPILE_OPTIONS_C_RELEASE}>")