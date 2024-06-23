# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(xkbcommon_FRAMEWORKS_FOUND_DEBUG "") # Will be filled later
conan_find_apple_frameworks(xkbcommon_FRAMEWORKS_FOUND_DEBUG "${xkbcommon_FRAMEWORKS_DEBUG}" "${xkbcommon_FRAMEWORK_DIRS_DEBUG}")

set(xkbcommon_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET xkbcommon_DEPS_TARGET)
    add_library(xkbcommon_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET xkbcommon_DEPS_TARGET
             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Debug>:${xkbcommon_FRAMEWORKS_FOUND_DEBUG}>
             $<$<CONFIG:Debug>:${xkbcommon_SYSTEM_LIBS_DEBUG}>
             $<$<CONFIG:Debug>:xkeyboard-config::xkeyboard-config;xkbcommon::libxkbcommon;xorg::xcb;xorg::xcb-xkb;LibXml2::LibXml2;wayland::wayland-client>)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### xkbcommon_DEPS_TARGET to all of them
conan_package_library_targets("${xkbcommon_LIBS_DEBUG}"    # libraries
                              "${xkbcommon_LIB_DIRS_DEBUG}" # package_libdir
                              "${xkbcommon_BIN_DIRS_DEBUG}" # package_bindir
                              "${xkbcommon_LIBRARY_TYPE_DEBUG}"
                              "${xkbcommon_IS_HOST_WINDOWS_DEBUG}"
                              xkbcommon_DEPS_TARGET
                              xkbcommon_LIBRARIES_TARGETS  # out_libraries_targets
                              "_DEBUG"
                              "xkbcommon"    # package_name
                              "${xkbcommon_NO_SONAME_MODE_DEBUG}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${xkbcommon_BUILD_DIRS_DEBUG} ${CMAKE_MODULE_PATH})

########## COMPONENTS TARGET PROPERTIES Debug ########################################

    ########## COMPONENT xkbcommon::xkbcli-interactive-wayland #############

        set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_FRAMEWORKS_FOUND_DEBUG "")
        conan_find_apple_frameworks(xkbcommon_xkbcommon_xkbcli-interactive-wayland_FRAMEWORKS_FOUND_DEBUG "${xkbcommon_xkbcommon_xkbcli-interactive-wayland_FRAMEWORKS_DEBUG}" "${xkbcommon_xkbcommon_xkbcli-interactive-wayland_FRAMEWORK_DIRS_DEBUG}")

        set(xkbcommon_xkbcommon_xkbcli-interactive-wayland_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET xkbcommon_xkbcommon_xkbcli-interactive-wayland_DEPS_TARGET)
            add_library(xkbcommon_xkbcommon_xkbcli-interactive-wayland_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET xkbcommon_xkbcommon_xkbcli-interactive-wayland_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_xkbcli-interactive-wayland_FRAMEWORKS_FOUND_DEBUG}>
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_xkbcli-interactive-wayland_SYSTEM_LIBS_DEBUG}>
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_xkbcli-interactive-wayland_DEPENDENCIES_DEBUG}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'xkbcommon_xkbcommon_xkbcli-interactive-wayland_DEPS_TARGET' to all of them
        conan_package_library_targets("${xkbcommon_xkbcommon_xkbcli-interactive-wayland_LIBS_DEBUG}"
                              "${xkbcommon_xkbcommon_xkbcli-interactive-wayland_LIB_DIRS_DEBUG}"
                              "${xkbcommon_xkbcommon_xkbcli-interactive-wayland_BIN_DIRS_DEBUG}" # package_bindir
                              "${xkbcommon_xkbcommon_xkbcli-interactive-wayland_LIBRARY_TYPE_DEBUG}"
                              "${xkbcommon_xkbcommon_xkbcli-interactive-wayland_IS_HOST_WINDOWS_DEBUG}"
                              xkbcommon_xkbcommon_xkbcli-interactive-wayland_DEPS_TARGET
                              xkbcommon_xkbcommon_xkbcli-interactive-wayland_LIBRARIES_TARGETS
                              "_DEBUG"
                              "xkbcommon_xkbcommon_xkbcli-interactive-wayland"
                              "${xkbcommon_xkbcommon_xkbcli-interactive-wayland_NO_SONAME_MODE_DEBUG}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET xkbcommon::xkbcli-interactive-wayland
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_xkbcli-interactive-wayland_OBJECTS_DEBUG}>
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_xkbcli-interactive-wayland_LIBRARIES_TARGETS}>
                     )

        if("${xkbcommon_xkbcommon_xkbcli-interactive-wayland_LIBS_DEBUG}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET xkbcommon::xkbcli-interactive-wayland
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         xkbcommon_xkbcommon_xkbcli-interactive-wayland_DEPS_TARGET)
        endif()

        set_property(TARGET xkbcommon::xkbcli-interactive-wayland APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_xkbcli-interactive-wayland_LINKER_FLAGS_DEBUG}>)
        set_property(TARGET xkbcommon::xkbcli-interactive-wayland APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_xkbcli-interactive-wayland_INCLUDE_DIRS_DEBUG}>)
        set_property(TARGET xkbcommon::xkbcli-interactive-wayland APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_xkbcli-interactive-wayland_LIB_DIRS_DEBUG}>)
        set_property(TARGET xkbcommon::xkbcli-interactive-wayland APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_xkbcli-interactive-wayland_COMPILE_DEFINITIONS_DEBUG}>)
        set_property(TARGET xkbcommon::xkbcli-interactive-wayland APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_xkbcli-interactive-wayland_COMPILE_OPTIONS_DEBUG}>)

    ########## COMPONENT xkbcommon::libxkbregistry #############

        set(xkbcommon_xkbcommon_libxkbregistry_FRAMEWORKS_FOUND_DEBUG "")
        conan_find_apple_frameworks(xkbcommon_xkbcommon_libxkbregistry_FRAMEWORKS_FOUND_DEBUG "${xkbcommon_xkbcommon_libxkbregistry_FRAMEWORKS_DEBUG}" "${xkbcommon_xkbcommon_libxkbregistry_FRAMEWORK_DIRS_DEBUG}")

        set(xkbcommon_xkbcommon_libxkbregistry_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET xkbcommon_xkbcommon_libxkbregistry_DEPS_TARGET)
            add_library(xkbcommon_xkbcommon_libxkbregistry_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET xkbcommon_xkbcommon_libxkbregistry_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbregistry_FRAMEWORKS_FOUND_DEBUG}>
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbregistry_SYSTEM_LIBS_DEBUG}>
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbregistry_DEPENDENCIES_DEBUG}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'xkbcommon_xkbcommon_libxkbregistry_DEPS_TARGET' to all of them
        conan_package_library_targets("${xkbcommon_xkbcommon_libxkbregistry_LIBS_DEBUG}"
                              "${xkbcommon_xkbcommon_libxkbregistry_LIB_DIRS_DEBUG}"
                              "${xkbcommon_xkbcommon_libxkbregistry_BIN_DIRS_DEBUG}" # package_bindir
                              "${xkbcommon_xkbcommon_libxkbregistry_LIBRARY_TYPE_DEBUG}"
                              "${xkbcommon_xkbcommon_libxkbregistry_IS_HOST_WINDOWS_DEBUG}"
                              xkbcommon_xkbcommon_libxkbregistry_DEPS_TARGET
                              xkbcommon_xkbcommon_libxkbregistry_LIBRARIES_TARGETS
                              "_DEBUG"
                              "xkbcommon_xkbcommon_libxkbregistry"
                              "${xkbcommon_xkbcommon_libxkbregistry_NO_SONAME_MODE_DEBUG}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET xkbcommon::libxkbregistry
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbregistry_OBJECTS_DEBUG}>
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbregistry_LIBRARIES_TARGETS}>
                     )

        if("${xkbcommon_xkbcommon_libxkbregistry_LIBS_DEBUG}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET xkbcommon::libxkbregistry
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         xkbcommon_xkbcommon_libxkbregistry_DEPS_TARGET)
        endif()

        set_property(TARGET xkbcommon::libxkbregistry APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbregistry_LINKER_FLAGS_DEBUG}>)
        set_property(TARGET xkbcommon::libxkbregistry APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbregistry_INCLUDE_DIRS_DEBUG}>)
        set_property(TARGET xkbcommon::libxkbregistry APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbregistry_LIB_DIRS_DEBUG}>)
        set_property(TARGET xkbcommon::libxkbregistry APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbregistry_COMPILE_DEFINITIONS_DEBUG}>)
        set_property(TARGET xkbcommon::libxkbregistry APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbregistry_COMPILE_OPTIONS_DEBUG}>)

    ########## COMPONENT xkbcommon::libxkbcommon-x11 #############

        set(xkbcommon_xkbcommon_libxkbcommon-x11_FRAMEWORKS_FOUND_DEBUG "")
        conan_find_apple_frameworks(xkbcommon_xkbcommon_libxkbcommon-x11_FRAMEWORKS_FOUND_DEBUG "${xkbcommon_xkbcommon_libxkbcommon-x11_FRAMEWORKS_DEBUG}" "${xkbcommon_xkbcommon_libxkbcommon-x11_FRAMEWORK_DIRS_DEBUG}")

        set(xkbcommon_xkbcommon_libxkbcommon-x11_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET xkbcommon_xkbcommon_libxkbcommon-x11_DEPS_TARGET)
            add_library(xkbcommon_xkbcommon_libxkbcommon-x11_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET xkbcommon_xkbcommon_libxkbcommon-x11_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbcommon-x11_FRAMEWORKS_FOUND_DEBUG}>
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbcommon-x11_SYSTEM_LIBS_DEBUG}>
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbcommon-x11_DEPENDENCIES_DEBUG}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'xkbcommon_xkbcommon_libxkbcommon-x11_DEPS_TARGET' to all of them
        conan_package_library_targets("${xkbcommon_xkbcommon_libxkbcommon-x11_LIBS_DEBUG}"
                              "${xkbcommon_xkbcommon_libxkbcommon-x11_LIB_DIRS_DEBUG}"
                              "${xkbcommon_xkbcommon_libxkbcommon-x11_BIN_DIRS_DEBUG}" # package_bindir
                              "${xkbcommon_xkbcommon_libxkbcommon-x11_LIBRARY_TYPE_DEBUG}"
                              "${xkbcommon_xkbcommon_libxkbcommon-x11_IS_HOST_WINDOWS_DEBUG}"
                              xkbcommon_xkbcommon_libxkbcommon-x11_DEPS_TARGET
                              xkbcommon_xkbcommon_libxkbcommon-x11_LIBRARIES_TARGETS
                              "_DEBUG"
                              "xkbcommon_xkbcommon_libxkbcommon-x11"
                              "${xkbcommon_xkbcommon_libxkbcommon-x11_NO_SONAME_MODE_DEBUG}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET xkbcommon::libxkbcommon-x11
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbcommon-x11_OBJECTS_DEBUG}>
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbcommon-x11_LIBRARIES_TARGETS}>
                     )

        if("${xkbcommon_xkbcommon_libxkbcommon-x11_LIBS_DEBUG}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET xkbcommon::libxkbcommon-x11
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         xkbcommon_xkbcommon_libxkbcommon-x11_DEPS_TARGET)
        endif()

        set_property(TARGET xkbcommon::libxkbcommon-x11 APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbcommon-x11_LINKER_FLAGS_DEBUG}>)
        set_property(TARGET xkbcommon::libxkbcommon-x11 APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbcommon-x11_INCLUDE_DIRS_DEBUG}>)
        set_property(TARGET xkbcommon::libxkbcommon-x11 APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbcommon-x11_LIB_DIRS_DEBUG}>)
        set_property(TARGET xkbcommon::libxkbcommon-x11 APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbcommon-x11_COMPILE_DEFINITIONS_DEBUG}>)
        set_property(TARGET xkbcommon::libxkbcommon-x11 APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbcommon-x11_COMPILE_OPTIONS_DEBUG}>)

    ########## COMPONENT xkbcommon::libxkbcommon #############

        set(xkbcommon_xkbcommon_libxkbcommon_FRAMEWORKS_FOUND_DEBUG "")
        conan_find_apple_frameworks(xkbcommon_xkbcommon_libxkbcommon_FRAMEWORKS_FOUND_DEBUG "${xkbcommon_xkbcommon_libxkbcommon_FRAMEWORKS_DEBUG}" "${xkbcommon_xkbcommon_libxkbcommon_FRAMEWORK_DIRS_DEBUG}")

        set(xkbcommon_xkbcommon_libxkbcommon_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET xkbcommon_xkbcommon_libxkbcommon_DEPS_TARGET)
            add_library(xkbcommon_xkbcommon_libxkbcommon_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET xkbcommon_xkbcommon_libxkbcommon_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbcommon_FRAMEWORKS_FOUND_DEBUG}>
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbcommon_SYSTEM_LIBS_DEBUG}>
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbcommon_DEPENDENCIES_DEBUG}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'xkbcommon_xkbcommon_libxkbcommon_DEPS_TARGET' to all of them
        conan_package_library_targets("${xkbcommon_xkbcommon_libxkbcommon_LIBS_DEBUG}"
                              "${xkbcommon_xkbcommon_libxkbcommon_LIB_DIRS_DEBUG}"
                              "${xkbcommon_xkbcommon_libxkbcommon_BIN_DIRS_DEBUG}" # package_bindir
                              "${xkbcommon_xkbcommon_libxkbcommon_LIBRARY_TYPE_DEBUG}"
                              "${xkbcommon_xkbcommon_libxkbcommon_IS_HOST_WINDOWS_DEBUG}"
                              xkbcommon_xkbcommon_libxkbcommon_DEPS_TARGET
                              xkbcommon_xkbcommon_libxkbcommon_LIBRARIES_TARGETS
                              "_DEBUG"
                              "xkbcommon_xkbcommon_libxkbcommon"
                              "${xkbcommon_xkbcommon_libxkbcommon_NO_SONAME_MODE_DEBUG}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET xkbcommon::libxkbcommon
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbcommon_OBJECTS_DEBUG}>
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbcommon_LIBRARIES_TARGETS}>
                     )

        if("${xkbcommon_xkbcommon_libxkbcommon_LIBS_DEBUG}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET xkbcommon::libxkbcommon
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         xkbcommon_xkbcommon_libxkbcommon_DEPS_TARGET)
        endif()

        set_property(TARGET xkbcommon::libxkbcommon APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbcommon_LINKER_FLAGS_DEBUG}>)
        set_property(TARGET xkbcommon::libxkbcommon APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbcommon_INCLUDE_DIRS_DEBUG}>)
        set_property(TARGET xkbcommon::libxkbcommon APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbcommon_LIB_DIRS_DEBUG}>)
        set_property(TARGET xkbcommon::libxkbcommon APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbcommon_COMPILE_DEFINITIONS_DEBUG}>)
        set_property(TARGET xkbcommon::libxkbcommon APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Debug>:${xkbcommon_xkbcommon_libxkbcommon_COMPILE_OPTIONS_DEBUG}>)

    ########## AGGREGATED GLOBAL TARGET WITH THE COMPONENTS #####################
    set_property(TARGET xkbcommon::xkbcommon APPEND PROPERTY INTERFACE_LINK_LIBRARIES xkbcommon::xkbcli-interactive-wayland)
    set_property(TARGET xkbcommon::xkbcommon APPEND PROPERTY INTERFACE_LINK_LIBRARIES xkbcommon::libxkbregistry)
    set_property(TARGET xkbcommon::xkbcommon APPEND PROPERTY INTERFACE_LINK_LIBRARIES xkbcommon::libxkbcommon-x11)
    set_property(TARGET xkbcommon::xkbcommon APPEND PROPERTY INTERFACE_LINK_LIBRARIES xkbcommon::libxkbcommon)

########## For the modules (FindXXX)
set(xkbcommon_LIBRARIES_DEBUG xkbcommon::xkbcommon)
