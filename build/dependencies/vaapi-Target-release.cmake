# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(vaapi_FRAMEWORKS_FOUND_RELEASE "") # Will be filled later
conan_find_apple_frameworks(vaapi_FRAMEWORKS_FOUND_RELEASE "${vaapi_FRAMEWORKS_RELEASE}" "${vaapi_FRAMEWORK_DIRS_RELEASE}")

set(vaapi_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET vaapi_DEPS_TARGET)
    add_library(vaapi_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET vaapi_DEPS_TARGET
             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Release>:${vaapi_FRAMEWORKS_FOUND_RELEASE}>
             $<$<CONFIG:Release>:${vaapi_SYSTEM_LIBS_RELEASE}>
             $<$<CONFIG:Release>:>)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### vaapi_DEPS_TARGET to all of them
conan_package_library_targets("${vaapi_LIBS_RELEASE}"    # libraries
                              "${vaapi_LIB_DIRS_RELEASE}" # package_libdir
                              "${vaapi_BIN_DIRS_RELEASE}" # package_bindir
                              "${vaapi_LIBRARY_TYPE_RELEASE}"
                              "${vaapi_IS_HOST_WINDOWS_RELEASE}"
                              vaapi_DEPS_TARGET
                              vaapi_LIBRARIES_TARGETS  # out_libraries_targets
                              "_RELEASE"
                              "vaapi"    # package_name
                              "${vaapi_NO_SONAME_MODE_RELEASE}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${vaapi_BUILD_DIRS_RELEASE} ${CMAKE_MODULE_PATH})

########## COMPONENTS TARGET PROPERTIES Release ########################################

    ########## COMPONENT vaapi::libva-drm #############

        set(vaapi_vaapi_libva-drm_FRAMEWORKS_FOUND_RELEASE "")
        conan_find_apple_frameworks(vaapi_vaapi_libva-drm_FRAMEWORKS_FOUND_RELEASE "${vaapi_vaapi_libva-drm_FRAMEWORKS_RELEASE}" "${vaapi_vaapi_libva-drm_FRAMEWORK_DIRS_RELEASE}")

        set(vaapi_vaapi_libva-drm_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET vaapi_vaapi_libva-drm_DEPS_TARGET)
            add_library(vaapi_vaapi_libva-drm_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET vaapi_vaapi_libva-drm_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva-drm_FRAMEWORKS_FOUND_RELEASE}>
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva-drm_SYSTEM_LIBS_RELEASE}>
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva-drm_DEPENDENCIES_RELEASE}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'vaapi_vaapi_libva-drm_DEPS_TARGET' to all of them
        conan_package_library_targets("${vaapi_vaapi_libva-drm_LIBS_RELEASE}"
                              "${vaapi_vaapi_libva-drm_LIB_DIRS_RELEASE}"
                              "${vaapi_vaapi_libva-drm_BIN_DIRS_RELEASE}" # package_bindir
                              "${vaapi_vaapi_libva-drm_LIBRARY_TYPE_RELEASE}"
                              "${vaapi_vaapi_libva-drm_IS_HOST_WINDOWS_RELEASE}"
                              vaapi_vaapi_libva-drm_DEPS_TARGET
                              vaapi_vaapi_libva-drm_LIBRARIES_TARGETS
                              "_RELEASE"
                              "vaapi_vaapi_libva-drm"
                              "${vaapi_vaapi_libva-drm_NO_SONAME_MODE_RELEASE}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET vaapi::libva-drm
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva-drm_OBJECTS_RELEASE}>
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva-drm_LIBRARIES_TARGETS}>
                     )

        if("${vaapi_vaapi_libva-drm_LIBS_RELEASE}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET vaapi::libva-drm
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         vaapi_vaapi_libva-drm_DEPS_TARGET)
        endif()

        set_property(TARGET vaapi::libva-drm APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva-drm_LINKER_FLAGS_RELEASE}>)
        set_property(TARGET vaapi::libva-drm APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva-drm_INCLUDE_DIRS_RELEASE}>)
        set_property(TARGET vaapi::libva-drm APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva-drm_LIB_DIRS_RELEASE}>)
        set_property(TARGET vaapi::libva-drm APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva-drm_COMPILE_DEFINITIONS_RELEASE}>)
        set_property(TARGET vaapi::libva-drm APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva-drm_COMPILE_OPTIONS_RELEASE}>)

    ########## COMPONENT vaapi::libva-x11 #############

        set(vaapi_vaapi_libva-x11_FRAMEWORKS_FOUND_RELEASE "")
        conan_find_apple_frameworks(vaapi_vaapi_libva-x11_FRAMEWORKS_FOUND_RELEASE "${vaapi_vaapi_libva-x11_FRAMEWORKS_RELEASE}" "${vaapi_vaapi_libva-x11_FRAMEWORK_DIRS_RELEASE}")

        set(vaapi_vaapi_libva-x11_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET vaapi_vaapi_libva-x11_DEPS_TARGET)
            add_library(vaapi_vaapi_libva-x11_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET vaapi_vaapi_libva-x11_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva-x11_FRAMEWORKS_FOUND_RELEASE}>
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva-x11_SYSTEM_LIBS_RELEASE}>
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva-x11_DEPENDENCIES_RELEASE}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'vaapi_vaapi_libva-x11_DEPS_TARGET' to all of them
        conan_package_library_targets("${vaapi_vaapi_libva-x11_LIBS_RELEASE}"
                              "${vaapi_vaapi_libva-x11_LIB_DIRS_RELEASE}"
                              "${vaapi_vaapi_libva-x11_BIN_DIRS_RELEASE}" # package_bindir
                              "${vaapi_vaapi_libva-x11_LIBRARY_TYPE_RELEASE}"
                              "${vaapi_vaapi_libva-x11_IS_HOST_WINDOWS_RELEASE}"
                              vaapi_vaapi_libva-x11_DEPS_TARGET
                              vaapi_vaapi_libva-x11_LIBRARIES_TARGETS
                              "_RELEASE"
                              "vaapi_vaapi_libva-x11"
                              "${vaapi_vaapi_libva-x11_NO_SONAME_MODE_RELEASE}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET vaapi::libva-x11
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva-x11_OBJECTS_RELEASE}>
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva-x11_LIBRARIES_TARGETS}>
                     )

        if("${vaapi_vaapi_libva-x11_LIBS_RELEASE}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET vaapi::libva-x11
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         vaapi_vaapi_libva-x11_DEPS_TARGET)
        endif()

        set_property(TARGET vaapi::libva-x11 APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva-x11_LINKER_FLAGS_RELEASE}>)
        set_property(TARGET vaapi::libva-x11 APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva-x11_INCLUDE_DIRS_RELEASE}>)
        set_property(TARGET vaapi::libva-x11 APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva-x11_LIB_DIRS_RELEASE}>)
        set_property(TARGET vaapi::libva-x11 APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva-x11_COMPILE_DEFINITIONS_RELEASE}>)
        set_property(TARGET vaapi::libva-x11 APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva-x11_COMPILE_OPTIONS_RELEASE}>)

    ########## COMPONENT vaapi::libva #############

        set(vaapi_vaapi_libva_FRAMEWORKS_FOUND_RELEASE "")
        conan_find_apple_frameworks(vaapi_vaapi_libva_FRAMEWORKS_FOUND_RELEASE "${vaapi_vaapi_libva_FRAMEWORKS_RELEASE}" "${vaapi_vaapi_libva_FRAMEWORK_DIRS_RELEASE}")

        set(vaapi_vaapi_libva_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET vaapi_vaapi_libva_DEPS_TARGET)
            add_library(vaapi_vaapi_libva_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET vaapi_vaapi_libva_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva_FRAMEWORKS_FOUND_RELEASE}>
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva_SYSTEM_LIBS_RELEASE}>
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva_DEPENDENCIES_RELEASE}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'vaapi_vaapi_libva_DEPS_TARGET' to all of them
        conan_package_library_targets("${vaapi_vaapi_libva_LIBS_RELEASE}"
                              "${vaapi_vaapi_libva_LIB_DIRS_RELEASE}"
                              "${vaapi_vaapi_libva_BIN_DIRS_RELEASE}" # package_bindir
                              "${vaapi_vaapi_libva_LIBRARY_TYPE_RELEASE}"
                              "${vaapi_vaapi_libva_IS_HOST_WINDOWS_RELEASE}"
                              vaapi_vaapi_libva_DEPS_TARGET
                              vaapi_vaapi_libva_LIBRARIES_TARGETS
                              "_RELEASE"
                              "vaapi_vaapi_libva"
                              "${vaapi_vaapi_libva_NO_SONAME_MODE_RELEASE}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET vaapi::libva
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva_OBJECTS_RELEASE}>
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva_LIBRARIES_TARGETS}>
                     )

        if("${vaapi_vaapi_libva_LIBS_RELEASE}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET vaapi::libva
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         vaapi_vaapi_libva_DEPS_TARGET)
        endif()

        set_property(TARGET vaapi::libva APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva_LINKER_FLAGS_RELEASE}>)
        set_property(TARGET vaapi::libva APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva_INCLUDE_DIRS_RELEASE}>)
        set_property(TARGET vaapi::libva APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva_LIB_DIRS_RELEASE}>)
        set_property(TARGET vaapi::libva APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva_COMPILE_DEFINITIONS_RELEASE}>)
        set_property(TARGET vaapi::libva APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Release>:${vaapi_vaapi_libva_COMPILE_OPTIONS_RELEASE}>)

    ########## AGGREGATED GLOBAL TARGET WITH THE COMPONENTS #####################
    set_property(TARGET vaapi::vaapi APPEND PROPERTY INTERFACE_LINK_LIBRARIES vaapi::libva-drm)
    set_property(TARGET vaapi::vaapi APPEND PROPERTY INTERFACE_LINK_LIBRARIES vaapi::libva-x11)
    set_property(TARGET vaapi::vaapi APPEND PROPERTY INTERFACE_LINK_LIBRARIES vaapi::libva)

########## For the modules (FindXXX)
set(vaapi_LIBRARIES_RELEASE vaapi::vaapi)
