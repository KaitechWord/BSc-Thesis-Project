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
set(vaapi_PACKAGE_FOLDER_DEBUG "/home/kajtekk/.conan2/p/vaapiee72fb83987e1/p")
set(vaapi_BUILD_MODULES_PATHS_DEBUG )


set(vaapi_INCLUDE_DIRS_DEBUG )
set(vaapi_RES_DIRS_DEBUG )
set(vaapi_DEFINITIONS_DEBUG )
set(vaapi_SHARED_LINK_FLAGS_DEBUG )
set(vaapi_EXE_LINK_FLAGS_DEBUG )
set(vaapi_OBJECTS_DEBUG )
set(vaapi_COMPILE_DEFINITIONS_DEBUG )
set(vaapi_COMPILE_OPTIONS_C_DEBUG )
set(vaapi_COMPILE_OPTIONS_CXX_DEBUG )
set(vaapi_LIB_DIRS_DEBUG )
set(vaapi_BIN_DIRS_DEBUG "${vaapi_PACKAGE_FOLDER_DEBUG}/bin")
set(vaapi_LIBRARY_TYPE_DEBUG SHARED)
set(vaapi_IS_HOST_WINDOWS_DEBUG 0)
set(vaapi_LIBS_DEBUG )
set(vaapi_SYSTEM_LIBS_DEBUG va-drm va va-x11)
set(vaapi_FRAMEWORK_DIRS_DEBUG )
set(vaapi_FRAMEWORKS_DEBUG )
set(vaapi_BUILD_DIRS_DEBUG )
set(vaapi_NO_SONAME_MODE_DEBUG FALSE)


# COMPOUND VARIABLES
set(vaapi_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${vaapi_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${vaapi_COMPILE_OPTIONS_C_DEBUG}>")
set(vaapi_LINKER_FLAGS_DEBUG
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${vaapi_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${vaapi_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${vaapi_EXE_LINK_FLAGS_DEBUG}>")


set(vaapi_COMPONENTS_DEBUG vaapi::libva vaapi::libva-x11 vaapi::libva-drm)
########### COMPONENT vaapi::libva-drm VARIABLES ############################################

set(vaapi_vaapi_libva-drm_INCLUDE_DIRS_DEBUG )
set(vaapi_vaapi_libva-drm_LIB_DIRS_DEBUG )
set(vaapi_vaapi_libva-drm_BIN_DIRS_DEBUG "${vaapi_PACKAGE_FOLDER_DEBUG}/bin")
set(vaapi_vaapi_libva-drm_LIBRARY_TYPE_DEBUG SHARED)
set(vaapi_vaapi_libva-drm_IS_HOST_WINDOWS_DEBUG 0)
set(vaapi_vaapi_libva-drm_RES_DIRS_DEBUG )
set(vaapi_vaapi_libva-drm_DEFINITIONS_DEBUG )
set(vaapi_vaapi_libva-drm_OBJECTS_DEBUG )
set(vaapi_vaapi_libva-drm_COMPILE_DEFINITIONS_DEBUG )
set(vaapi_vaapi_libva-drm_COMPILE_OPTIONS_C_DEBUG "")
set(vaapi_vaapi_libva-drm_COMPILE_OPTIONS_CXX_DEBUG "")
set(vaapi_vaapi_libva-drm_LIBS_DEBUG )
set(vaapi_vaapi_libva-drm_SYSTEM_LIBS_DEBUG va-drm va)
set(vaapi_vaapi_libva-drm_FRAMEWORK_DIRS_DEBUG )
set(vaapi_vaapi_libva-drm_FRAMEWORKS_DEBUG )
set(vaapi_vaapi_libva-drm_DEPENDENCIES_DEBUG )
set(vaapi_vaapi_libva-drm_SHARED_LINK_FLAGS_DEBUG )
set(vaapi_vaapi_libva-drm_EXE_LINK_FLAGS_DEBUG )
set(vaapi_vaapi_libva-drm_NO_SONAME_MODE_DEBUG FALSE)

# COMPOUND VARIABLES
set(vaapi_vaapi_libva-drm_LINKER_FLAGS_DEBUG
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${vaapi_vaapi_libva-drm_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${vaapi_vaapi_libva-drm_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${vaapi_vaapi_libva-drm_EXE_LINK_FLAGS_DEBUG}>
)
set(vaapi_vaapi_libva-drm_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${vaapi_vaapi_libva-drm_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${vaapi_vaapi_libva-drm_COMPILE_OPTIONS_C_DEBUG}>")
########### COMPONENT vaapi::libva-x11 VARIABLES ############################################

set(vaapi_vaapi_libva-x11_INCLUDE_DIRS_DEBUG )
set(vaapi_vaapi_libva-x11_LIB_DIRS_DEBUG )
set(vaapi_vaapi_libva-x11_BIN_DIRS_DEBUG "${vaapi_PACKAGE_FOLDER_DEBUG}/bin")
set(vaapi_vaapi_libva-x11_LIBRARY_TYPE_DEBUG SHARED)
set(vaapi_vaapi_libva-x11_IS_HOST_WINDOWS_DEBUG 0)
set(vaapi_vaapi_libva-x11_RES_DIRS_DEBUG )
set(vaapi_vaapi_libva-x11_DEFINITIONS_DEBUG )
set(vaapi_vaapi_libva-x11_OBJECTS_DEBUG )
set(vaapi_vaapi_libva-x11_COMPILE_DEFINITIONS_DEBUG )
set(vaapi_vaapi_libva-x11_COMPILE_OPTIONS_C_DEBUG "")
set(vaapi_vaapi_libva-x11_COMPILE_OPTIONS_CXX_DEBUG "")
set(vaapi_vaapi_libva-x11_LIBS_DEBUG )
set(vaapi_vaapi_libva-x11_SYSTEM_LIBS_DEBUG va-x11 va)
set(vaapi_vaapi_libva-x11_FRAMEWORK_DIRS_DEBUG )
set(vaapi_vaapi_libva-x11_FRAMEWORKS_DEBUG )
set(vaapi_vaapi_libva-x11_DEPENDENCIES_DEBUG )
set(vaapi_vaapi_libva-x11_SHARED_LINK_FLAGS_DEBUG )
set(vaapi_vaapi_libva-x11_EXE_LINK_FLAGS_DEBUG )
set(vaapi_vaapi_libva-x11_NO_SONAME_MODE_DEBUG FALSE)

# COMPOUND VARIABLES
set(vaapi_vaapi_libva-x11_LINKER_FLAGS_DEBUG
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${vaapi_vaapi_libva-x11_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${vaapi_vaapi_libva-x11_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${vaapi_vaapi_libva-x11_EXE_LINK_FLAGS_DEBUG}>
)
set(vaapi_vaapi_libva-x11_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${vaapi_vaapi_libva-x11_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${vaapi_vaapi_libva-x11_COMPILE_OPTIONS_C_DEBUG}>")
########### COMPONENT vaapi::libva VARIABLES ############################################

set(vaapi_vaapi_libva_INCLUDE_DIRS_DEBUG )
set(vaapi_vaapi_libva_LIB_DIRS_DEBUG )
set(vaapi_vaapi_libva_BIN_DIRS_DEBUG "${vaapi_PACKAGE_FOLDER_DEBUG}/bin")
set(vaapi_vaapi_libva_LIBRARY_TYPE_DEBUG SHARED)
set(vaapi_vaapi_libva_IS_HOST_WINDOWS_DEBUG 0)
set(vaapi_vaapi_libva_RES_DIRS_DEBUG )
set(vaapi_vaapi_libva_DEFINITIONS_DEBUG )
set(vaapi_vaapi_libva_OBJECTS_DEBUG )
set(vaapi_vaapi_libva_COMPILE_DEFINITIONS_DEBUG )
set(vaapi_vaapi_libva_COMPILE_OPTIONS_C_DEBUG "")
set(vaapi_vaapi_libva_COMPILE_OPTIONS_CXX_DEBUG "")
set(vaapi_vaapi_libva_LIBS_DEBUG )
set(vaapi_vaapi_libva_SYSTEM_LIBS_DEBUG va)
set(vaapi_vaapi_libva_FRAMEWORK_DIRS_DEBUG )
set(vaapi_vaapi_libva_FRAMEWORKS_DEBUG )
set(vaapi_vaapi_libva_DEPENDENCIES_DEBUG )
set(vaapi_vaapi_libva_SHARED_LINK_FLAGS_DEBUG )
set(vaapi_vaapi_libva_EXE_LINK_FLAGS_DEBUG )
set(vaapi_vaapi_libva_NO_SONAME_MODE_DEBUG FALSE)

# COMPOUND VARIABLES
set(vaapi_vaapi_libva_LINKER_FLAGS_DEBUG
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${vaapi_vaapi_libva_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${vaapi_vaapi_libva_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${vaapi_vaapi_libva_EXE_LINK_FLAGS_DEBUG}>
)
set(vaapi_vaapi_libva_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${vaapi_vaapi_libva_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${vaapi_vaapi_libva_COMPILE_OPTIONS_C_DEBUG}>")