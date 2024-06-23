########### AGGREGATED COMPONENTS AND DEPENDENCIES FOR THE MULTI CONFIG #####################
#############################################################################################

list(APPEND xkbcommon_COMPONENT_NAMES xkbcommon::libxkbcommon xkbcommon::libxkbcommon-x11 xkbcommon::libxkbregistry xkbcommon::xkbcli-interactive-wayland)
list(REMOVE_DUPLICATES xkbcommon_COMPONENT_NAMES)
if(DEFINED xkbcommon_FIND_DEPENDENCY_NAMES)
  list(APPEND xkbcommon_FIND_DEPENDENCY_NAMES xkeyboard-config wayland libxml2 xorg)
  list(REMOVE_DUPLICATES xkbcommon_FIND_DEPENDENCY_NAMES)
else()
  set(xkbcommon_FIND_DEPENDENCY_NAMES xkeyboard-config wayland libxml2 xorg)
endif()
set(xkeyboard-config_FIND_MODE "NO_MODULE")
set(wayland_FIND_MODE "NO_MODULE")
set(libxml2_FIND_MODE "NO_MODULE")
set(xorg_FIND_MODE "NO_MODULE")

########### VARIABLES #######################################################################
#############################################################################################
set(xkbcommon_PACKAGE_FOLDER_DEBUG "/home/kajtekk/.conan2/p/b/xkbco82ebf4d50669b/p")
set(xkbcommon_BUILD_MODULES_PATHS_DEBUG )


set(xkbcommon_INCLUDE_DIRS_DEBUG )
set(xkbcommon_RES_DIRS_DEBUG "${xkbcommon_PACKAGE_FOLDER_DEBUG}/res")
set(xkbcommon_DEFINITIONS_DEBUG )
set(xkbcommon_SHARED_LINK_FLAGS_DEBUG )
set(xkbcommon_EXE_LINK_FLAGS_DEBUG )
set(xkbcommon_OBJECTS_DEBUG )
set(xkbcommon_COMPILE_DEFINITIONS_DEBUG )
set(xkbcommon_COMPILE_OPTIONS_C_DEBUG )
set(xkbcommon_COMPILE_OPTIONS_CXX_DEBUG )
set(xkbcommon_LIB_DIRS_DEBUG "${xkbcommon_PACKAGE_FOLDER_DEBUG}/lib")
set(xkbcommon_BIN_DIRS_DEBUG )
set(xkbcommon_LIBRARY_TYPE_DEBUG STATIC)
set(xkbcommon_IS_HOST_WINDOWS_DEBUG 0)
set(xkbcommon_LIBS_DEBUG xkbregistry xkbcommon-x11 xkbcommon)
set(xkbcommon_SYSTEM_LIBS_DEBUG )
set(xkbcommon_FRAMEWORK_DIRS_DEBUG )
set(xkbcommon_FRAMEWORKS_DEBUG )
set(xkbcommon_BUILD_DIRS_DEBUG )
set(xkbcommon_NO_SONAME_MODE_DEBUG FALSE)


# COMPOUND VARIABLES
set(xkbcommon_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${xkbcommon_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${xkbcommon_COMPILE_OPTIONS_C_DEBUG}>")
set(xkbcommon_LINKER_FLAGS_DEBUG
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${xkbcommon_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${xkbcommon_SHARED_LINK_FLAGS_DEBUG}>"
    "$<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${xkbcommon_EXE_LINK_FLAGS_DEBUG}>")


set(xkbcommon_COMPONENTS_DEBUG xkbcommon::libxkbcommon xkbcommon::libxkbcommon-x11 xkbcommon::libxkbregistry xkbcommon::xkbcli-interactive-wayland)
########### COMPONENT xkbcommon::xkbcli-interactive-wayland VARIABLES ############################################

set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_INCLUDE_DIRS_DEBUG )
set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_LIB_DIRS_DEBUG "${xkbcommon_PACKAGE_FOLDER_DEBUG}/lib")
set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_BIN_DIRS_DEBUG )
set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_LIBRARY_TYPE_DEBUG STATIC)
set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_IS_HOST_WINDOWS_DEBUG 0)
set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_RES_DIRS_DEBUG )
set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_DEFINITIONS_DEBUG )
set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_OBJECTS_DEBUG )
set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_COMPILE_DEFINITIONS_DEBUG )
set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_COMPILE_OPTIONS_C_DEBUG "")
set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_COMPILE_OPTIONS_CXX_DEBUG "")
set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_LIBS_DEBUG )
set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_SYSTEM_LIBS_DEBUG )
set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_FRAMEWORK_DIRS_DEBUG )
set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_FRAMEWORKS_DEBUG )
set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_DEPENDENCIES_DEBUG wayland::wayland-client)
set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_SHARED_LINK_FLAGS_DEBUG )
set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_EXE_LINK_FLAGS_DEBUG )
set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_NO_SONAME_MODE_DEBUG FALSE)

# COMPOUND VARIABLES
set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_LINKER_FLAGS_DEBUG
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${xkbcommon_xkbcommon_xkbcli-interactive-wayland_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${xkbcommon_xkbcommon_xkbcli-interactive-wayland_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${xkbcommon_xkbcommon_xkbcli-interactive-wayland_EXE_LINK_FLAGS_DEBUG}>
)
set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${xkbcommon_xkbcommon_xkbcli-interactive-wayland_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${xkbcommon_xkbcommon_xkbcli-interactive-wayland_COMPILE_OPTIONS_C_DEBUG}>")
########### COMPONENT xkbcommon::libxkbregistry VARIABLES ############################################

set(xkbcommon_xkbcommon_libxkbregistry_INCLUDE_DIRS_DEBUG )
set(xkbcommon_xkbcommon_libxkbregistry_LIB_DIRS_DEBUG "${xkbcommon_PACKAGE_FOLDER_DEBUG}/lib")
set(xkbcommon_xkbcommon_libxkbregistry_BIN_DIRS_DEBUG )
set(xkbcommon_xkbcommon_libxkbregistry_LIBRARY_TYPE_DEBUG STATIC)
set(xkbcommon_xkbcommon_libxkbregistry_IS_HOST_WINDOWS_DEBUG 0)
set(xkbcommon_xkbcommon_libxkbregistry_RES_DIRS_DEBUG )
set(xkbcommon_xkbcommon_libxkbregistry_DEFINITIONS_DEBUG )
set(xkbcommon_xkbcommon_libxkbregistry_OBJECTS_DEBUG )
set(xkbcommon_xkbcommon_libxkbregistry_COMPILE_DEFINITIONS_DEBUG )
set(xkbcommon_xkbcommon_libxkbregistry_COMPILE_OPTIONS_C_DEBUG "")
set(xkbcommon_xkbcommon_libxkbregistry_COMPILE_OPTIONS_CXX_DEBUG "")
set(xkbcommon_xkbcommon_libxkbregistry_LIBS_DEBUG xkbregistry)
set(xkbcommon_xkbcommon_libxkbregistry_SYSTEM_LIBS_DEBUG )
set(xkbcommon_xkbcommon_libxkbregistry_FRAMEWORK_DIRS_DEBUG )
set(xkbcommon_xkbcommon_libxkbregistry_FRAMEWORKS_DEBUG )
set(xkbcommon_xkbcommon_libxkbregistry_DEPENDENCIES_DEBUG LibXml2::LibXml2)
set(xkbcommon_xkbcommon_libxkbregistry_SHARED_LINK_FLAGS_DEBUG )
set(xkbcommon_xkbcommon_libxkbregistry_EXE_LINK_FLAGS_DEBUG )
set(xkbcommon_xkbcommon_libxkbregistry_NO_SONAME_MODE_DEBUG FALSE)

# COMPOUND VARIABLES
set(xkbcommon_xkbcommon_libxkbregistry_LINKER_FLAGS_DEBUG
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${xkbcommon_xkbcommon_libxkbregistry_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${xkbcommon_xkbcommon_libxkbregistry_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${xkbcommon_xkbcommon_libxkbregistry_EXE_LINK_FLAGS_DEBUG}>
)
set(xkbcommon_xkbcommon_libxkbregistry_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${xkbcommon_xkbcommon_libxkbregistry_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${xkbcommon_xkbcommon_libxkbregistry_COMPILE_OPTIONS_C_DEBUG}>")
########### COMPONENT xkbcommon::libxkbcommon-x11 VARIABLES ############################################

set(xkbcommon_xkbcommon_libxkbcommon-x11_INCLUDE_DIRS_DEBUG )
set(xkbcommon_xkbcommon_libxkbcommon-x11_LIB_DIRS_DEBUG "${xkbcommon_PACKAGE_FOLDER_DEBUG}/lib")
set(xkbcommon_xkbcommon_libxkbcommon-x11_BIN_DIRS_DEBUG )
set(xkbcommon_xkbcommon_libxkbcommon-x11_LIBRARY_TYPE_DEBUG STATIC)
set(xkbcommon_xkbcommon_libxkbcommon-x11_IS_HOST_WINDOWS_DEBUG 0)
set(xkbcommon_xkbcommon_libxkbcommon-x11_RES_DIRS_DEBUG )
set(xkbcommon_xkbcommon_libxkbcommon-x11_DEFINITIONS_DEBUG )
set(xkbcommon_xkbcommon_libxkbcommon-x11_OBJECTS_DEBUG )
set(xkbcommon_xkbcommon_libxkbcommon-x11_COMPILE_DEFINITIONS_DEBUG )
set(xkbcommon_xkbcommon_libxkbcommon-x11_COMPILE_OPTIONS_C_DEBUG "")
set(xkbcommon_xkbcommon_libxkbcommon-x11_COMPILE_OPTIONS_CXX_DEBUG "")
set(xkbcommon_xkbcommon_libxkbcommon-x11_LIBS_DEBUG xkbcommon-x11)
set(xkbcommon_xkbcommon_libxkbcommon-x11_SYSTEM_LIBS_DEBUG )
set(xkbcommon_xkbcommon_libxkbcommon-x11_FRAMEWORK_DIRS_DEBUG )
set(xkbcommon_xkbcommon_libxkbcommon-x11_FRAMEWORKS_DEBUG )
set(xkbcommon_xkbcommon_libxkbcommon-x11_DEPENDENCIES_DEBUG xkbcommon::libxkbcommon xorg::xcb xorg::xcb-xkb)
set(xkbcommon_xkbcommon_libxkbcommon-x11_SHARED_LINK_FLAGS_DEBUG )
set(xkbcommon_xkbcommon_libxkbcommon-x11_EXE_LINK_FLAGS_DEBUG )
set(xkbcommon_xkbcommon_libxkbcommon-x11_NO_SONAME_MODE_DEBUG FALSE)

# COMPOUND VARIABLES
set(xkbcommon_xkbcommon_libxkbcommon-x11_LINKER_FLAGS_DEBUG
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${xkbcommon_xkbcommon_libxkbcommon-x11_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${xkbcommon_xkbcommon_libxkbcommon-x11_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${xkbcommon_xkbcommon_libxkbcommon-x11_EXE_LINK_FLAGS_DEBUG}>
)
set(xkbcommon_xkbcommon_libxkbcommon-x11_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${xkbcommon_xkbcommon_libxkbcommon-x11_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${xkbcommon_xkbcommon_libxkbcommon-x11_COMPILE_OPTIONS_C_DEBUG}>")
########### COMPONENT xkbcommon::libxkbcommon VARIABLES ############################################

set(xkbcommon_xkbcommon_libxkbcommon_INCLUDE_DIRS_DEBUG )
set(xkbcommon_xkbcommon_libxkbcommon_LIB_DIRS_DEBUG "${xkbcommon_PACKAGE_FOLDER_DEBUG}/lib")
set(xkbcommon_xkbcommon_libxkbcommon_BIN_DIRS_DEBUG )
set(xkbcommon_xkbcommon_libxkbcommon_LIBRARY_TYPE_DEBUG STATIC)
set(xkbcommon_xkbcommon_libxkbcommon_IS_HOST_WINDOWS_DEBUG 0)
set(xkbcommon_xkbcommon_libxkbcommon_RES_DIRS_DEBUG "${xkbcommon_PACKAGE_FOLDER_DEBUG}/res")
set(xkbcommon_xkbcommon_libxkbcommon_DEFINITIONS_DEBUG )
set(xkbcommon_xkbcommon_libxkbcommon_OBJECTS_DEBUG )
set(xkbcommon_xkbcommon_libxkbcommon_COMPILE_DEFINITIONS_DEBUG )
set(xkbcommon_xkbcommon_libxkbcommon_COMPILE_OPTIONS_C_DEBUG "")
set(xkbcommon_xkbcommon_libxkbcommon_COMPILE_OPTIONS_CXX_DEBUG "")
set(xkbcommon_xkbcommon_libxkbcommon_LIBS_DEBUG xkbcommon)
set(xkbcommon_xkbcommon_libxkbcommon_SYSTEM_LIBS_DEBUG )
set(xkbcommon_xkbcommon_libxkbcommon_FRAMEWORK_DIRS_DEBUG )
set(xkbcommon_xkbcommon_libxkbcommon_FRAMEWORKS_DEBUG )
set(xkbcommon_xkbcommon_libxkbcommon_DEPENDENCIES_DEBUG xkeyboard-config::xkeyboard-config)
set(xkbcommon_xkbcommon_libxkbcommon_SHARED_LINK_FLAGS_DEBUG )
set(xkbcommon_xkbcommon_libxkbcommon_EXE_LINK_FLAGS_DEBUG )
set(xkbcommon_xkbcommon_libxkbcommon_NO_SONAME_MODE_DEBUG FALSE)

# COMPOUND VARIABLES
set(xkbcommon_xkbcommon_libxkbcommon_LINKER_FLAGS_DEBUG
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,SHARED_LIBRARY>:${xkbcommon_xkbcommon_libxkbcommon_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,MODULE_LIBRARY>:${xkbcommon_xkbcommon_libxkbcommon_SHARED_LINK_FLAGS_DEBUG}>
        $<$<STREQUAL:$<TARGET_PROPERTY:TYPE>,EXECUTABLE>:${xkbcommon_xkbcommon_libxkbcommon_EXE_LINK_FLAGS_DEBUG}>
)
set(xkbcommon_xkbcommon_libxkbcommon_COMPILE_OPTIONS_DEBUG
    "$<$<COMPILE_LANGUAGE:CXX>:${xkbcommon_xkbcommon_libxkbcommon_COMPILE_OPTIONS_CXX_DEBUG}>"
    "$<$<COMPILE_LANGUAGE:C>:${xkbcommon_xkbcommon_libxkbcommon_COMPILE_OPTIONS_C_DEBUG}>")