# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(vaapi_FRAMEWORKS_FOUND_DEBUG "") # Will be filled later
conan_find_apple_frameworks(vaapi_FRAMEWORKS_FOUND_DEBUG "${vaapi_FRAMEWORKS_DEBUG}" "${vaapi_FRAMEWORK_DIRS_DEBUG}")

set(vaapi_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET vaapi_DEPS_TARGET)
    add_library(vaapi_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET vaapi_DEPS_TARGET
             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Debug>:${vaapi_FRAMEWORKS_FOUND_DEBUG}>
             $<$<CONFIG:Debug>:${vaapi_SYSTEM_LIBS_DEBUG}>
             $<$<CONFIG:Debug>:>)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### vaapi_DEPS_TARGET to all of them
conan_package_library_targets("${vaapi_LIBS_DEBUG}"    # libraries
                              "${vaapi_LIB_DIRS_DEBUG}" # package_libdir
                              "${vaapi_BIN_DIRS_DEBUG}" # package_bindir
                              "${vaapi_LIBRARY_TYPE_DEBUG}"
                              "${vaapi_IS_HOST_WINDOWS_DEBUG}"
                              vaapi_DEPS_TARGET
                              vaapi_LIBRARIES_TARGETS  # out_libraries_targets
                              "_DEBUG"
                              "vaapi"    # package_name
                              "${vaapi_NO_SONAME_MODE_DEBUG}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${vaapi_BUILD_DIRS_DEBUG} ${CMAKE_MODULE_PATH})

########## COMPONENTS TARGET PROPERTIES Debug ########################################

    ########## COMPONENT vaapi::libva-drm #############

        set(vaapi_vaapi_libva-drm_FRAMEWORKS_FOUND_DEBUG "")
        conan_find_apple_frameworks(vaapi_vaapi_libva-drm_FRAMEWORKS_FOUND_DEBUG "${vaapi_vaapi_libva-drm_FRAMEWORKS_DEBUG}" "${vaapi_vaapi_libva-drm_FRAMEWORK_DIRS_DEBUG}")

        set(vaapi_vaapi_libva-drm_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET vaapi_vaapi_libva-drm_DEPS_TARGET)
            add_library(vaapi_vaapi_libva-drm_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET vaapi_vaapi_libva-drm_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva-drm_FRAMEWORKS_FOUND_DEBUG}>
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva-drm_SYSTEM_LIBS_DEBUG}>
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva-drm_DEPENDENCIES_DEBUG}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'vaapi_vaapi_libva-drm_DEPS_TARGET' to all of them
        conan_package_library_targets("${vaapi_vaapi_libva-drm_LIBS_DEBUG}"
                              "${vaapi_vaapi_libva-drm_LIB_DIRS_DEBUG}"
                              "${vaapi_vaapi_libva-drm_BIN_DIRS_DEBUG}" # package_bindir
                              "${vaapi_vaapi_libva-drm_LIBRARY_TYPE_DEBUG}"
                              "${vaapi_vaapi_libva-drm_IS_HOST_WINDOWS_DEBUG}"
                              vaapi_vaapi_libva-drm_DEPS_TARGET
                              vaapi_vaapi_libva-drm_LIBRARIES_TARGETS
                              "_DEBUG"
                              "vaapi_vaapi_libva-drm"
                              "${vaapi_vaapi_libva-drm_NO_SONAME_MODE_DEBUG}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET vaapi::libva-drm
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva-drm_OBJECTS_DEBUG}>
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva-drm_LIBRARIES_TARGETS}>
                     )

        if("${vaapi_vaapi_libva-drm_LIBS_DEBUG}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET vaapi::libva-drm
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         vaapi_vaapi_libva-drm_DEPS_TARGET)
        endif()

        set_property(TARGET vaapi::libva-drm APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva-drm_LINKER_FLAGS_DEBUG}>)
        set_property(TARGET vaapi::libva-drm APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva-drm_INCLUDE_DIRS_DEBUG}>)
        set_property(TARGET vaapi::libva-drm APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva-drm_LIB_DIRS_DEBUG}>)
        set_property(TARGET vaapi::libva-drm APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva-drm_COMPILE_DEFINITIONS_DEBUG}>)
        set_property(TARGET vaapi::libva-drm APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva-drm_COMPILE_OPTIONS_DEBUG}>)

    ########## COMPONENT vaapi::libva-x11 #############

        set(vaapi_vaapi_libva-x11_FRAMEWORKS_FOUND_DEBUG "")
        conan_find_apple_frameworks(vaapi_vaapi_libva-x11_FRAMEWORKS_FOUND_DEBUG "${vaapi_vaapi_libva-x11_FRAMEWORKS_DEBUG}" "${vaapi_vaapi_libva-x11_FRAMEWORK_DIRS_DEBUG}")

        set(vaapi_vaapi_libva-x11_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET vaapi_vaapi_libva-x11_DEPS_TARGET)
            add_library(vaapi_vaapi_libva-x11_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET vaapi_vaapi_libva-x11_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva-x11_FRAMEWORKS_FOUND_DEBUG}>
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva-x11_SYSTEM_LIBS_DEBUG}>
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva-x11_DEPENDENCIES_DEBUG}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'vaapi_vaapi_libva-x11_DEPS_TARGET' to all of them
        conan_package_library_targets("${vaapi_vaapi_libva-x11_LIBS_DEBUG}"
                              "${vaapi_vaapi_libva-x11_LIB_DIRS_DEBUG}"
                              "${vaapi_vaapi_libva-x11_BIN_DIRS_DEBUG}" # package_bindir
                              "${vaapi_vaapi_libva-x11_LIBRARY_TYPE_DEBUG}"
                              "${vaapi_vaapi_libva-x11_IS_HOST_WINDOWS_DEBUG}"
                              vaapi_vaapi_libva-x11_DEPS_TARGET
                              vaapi_vaapi_libva-x11_LIBRARIES_TARGETS
                              "_DEBUG"
                              "vaapi_vaapi_libva-x11"
                              "${vaapi_vaapi_libva-x11_NO_SONAME_MODE_DEBUG}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET vaapi::libva-x11
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva-x11_OBJECTS_DEBUG}>
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva-x11_LIBRARIES_TARGETS}>
                     )

        if("${vaapi_vaapi_libva-x11_LIBS_DEBUG}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET vaapi::libva-x11
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         vaapi_vaapi_libva-x11_DEPS_TARGET)
        endif()

        set_property(TARGET vaapi::libva-x11 APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva-x11_LINKER_FLAGS_DEBUG}>)
        set_property(TARGET vaapi::libva-x11 APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva-x11_INCLUDE_DIRS_DEBUG}>)
        set_property(TARGET vaapi::libva-x11 APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva-x11_LIB_DIRS_DEBUG}>)
        set_property(TARGET vaapi::libva-x11 APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva-x11_COMPILE_DEFINITIONS_DEBUG}>)
        set_property(TARGET vaapi::libva-x11 APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva-x11_COMPILE_OPTIONS_DEBUG}>)

    ########## COMPONENT vaapi::libva #############

        set(vaapi_vaapi_libva_FRAMEWORKS_FOUND_DEBUG "")
        conan_find_apple_frameworks(vaapi_vaapi_libva_FRAMEWORKS_FOUND_DEBUG "${vaapi_vaapi_libva_FRAMEWORKS_DEBUG}" "${vaapi_vaapi_libva_FRAMEWORK_DIRS_DEBUG}")

        set(vaapi_vaapi_libva_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET vaapi_vaapi_libva_DEPS_TARGET)
            add_library(vaapi_vaapi_libva_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET vaapi_vaapi_libva_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva_FRAMEWORKS_FOUND_DEBUG}>
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva_SYSTEM_LIBS_DEBUG}>
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva_DEPENDENCIES_DEBUG}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'vaapi_vaapi_libva_DEPS_TARGET' to all of them
        conan_package_library_targets("${vaapi_vaapi_libva_LIBS_DEBUG}"
                              "${vaapi_vaapi_libva_LIB_DIRS_DEBUG}"
                              "${vaapi_vaapi_libva_BIN_DIRS_DEBUG}" # package_bindir
                              "${vaapi_vaapi_libva_LIBRARY_TYPE_DEBUG}"
                              "${vaapi_vaapi_libva_IS_HOST_WINDOWS_DEBUG}"
                              vaapi_vaapi_libva_DEPS_TARGET
                              vaapi_vaapi_libva_LIBRARIES_TARGETS
                              "_DEBUG"
                              "vaapi_vaapi_libva"
                              "${vaapi_vaapi_libva_NO_SONAME_MODE_DEBUG}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET vaapi::libva
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva_OBJECTS_DEBUG}>
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva_LIBRARIES_TARGETS}>
                     )

        if("${vaapi_vaapi_libva_LIBS_DEBUG}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET vaapi::libva
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         vaapi_vaapi_libva_DEPS_TARGET)
        endif()

        set_property(TARGET vaapi::libva APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva_LINKER_FLAGS_DEBUG}>)
        set_property(TARGET vaapi::libva APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva_INCLUDE_DIRS_DEBUG}>)
        set_property(TARGET vaapi::libva APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva_LIB_DIRS_DEBUG}>)
        set_property(TARGET vaapi::libva APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva_COMPILE_DEFINITIONS_DEBUG}>)
        set_property(TARGET vaapi::libva APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Debug>:${vaapi_vaapi_libva_COMPILE_OPTIONS_DEBUG}>)

    ########## AGGREGATED GLOBAL TARGET WITH THE COMPONENTS #####################
    set_property(TARGET vaapi::vaapi APPEND PROPERTY INTERFACE_LINK_LIBRARIES vaapi::libva-drm)
    set_property(TARGET vaapi::vaapi APPEND PROPERTY INTERFACE_LINK_LIBRARIES vaapi::libva-x11)
    set_property(TARGET vaapi::vaapi APPEND PROPERTY INTERFACE_LINK_LIBRARIES vaapi::libva)

########## For the modules (FindXXX)
set(vaapi_LIBRARIES_DEBUG vaapi::vaapi)
