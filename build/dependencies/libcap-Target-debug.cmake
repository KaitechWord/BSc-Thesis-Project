# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(libcap_FRAMEWORKS_FOUND_DEBUG "") # Will be filled later
conan_find_apple_frameworks(libcap_FRAMEWORKS_FOUND_DEBUG "${libcap_FRAMEWORKS_DEBUG}" "${libcap_FRAMEWORK_DIRS_DEBUG}")

set(libcap_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET libcap_DEPS_TARGET)
    add_library(libcap_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET libcap_DEPS_TARGET
             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Debug>:${libcap_FRAMEWORKS_FOUND_DEBUG}>
             $<$<CONFIG:Debug>:${libcap_SYSTEM_LIBS_DEBUG}>
             $<$<CONFIG:Debug>:>)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### libcap_DEPS_TARGET to all of them
conan_package_library_targets("${libcap_LIBS_DEBUG}"    # libraries
                              "${libcap_LIB_DIRS_DEBUG}" # package_libdir
                              "${libcap_BIN_DIRS_DEBUG}" # package_bindir
                              "${libcap_LIBRARY_TYPE_DEBUG}"
                              "${libcap_IS_HOST_WINDOWS_DEBUG}"
                              libcap_DEPS_TARGET
                              libcap_LIBRARIES_TARGETS  # out_libraries_targets
                              "_DEBUG"
                              "libcap"    # package_name
                              "${libcap_NO_SONAME_MODE_DEBUG}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${libcap_BUILD_DIRS_DEBUG} ${CMAKE_MODULE_PATH})

########## COMPONENTS TARGET PROPERTIES Debug ########################################

    ########## COMPONENT libcap::cap #############

        set(libcap_libcap_cap_FRAMEWORKS_FOUND_DEBUG "")
        conan_find_apple_frameworks(libcap_libcap_cap_FRAMEWORKS_FOUND_DEBUG "${libcap_libcap_cap_FRAMEWORKS_DEBUG}" "${libcap_libcap_cap_FRAMEWORK_DIRS_DEBUG}")

        set(libcap_libcap_cap_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET libcap_libcap_cap_DEPS_TARGET)
            add_library(libcap_libcap_cap_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET libcap_libcap_cap_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${libcap_libcap_cap_FRAMEWORKS_FOUND_DEBUG}>
                     $<$<CONFIG:Debug>:${libcap_libcap_cap_SYSTEM_LIBS_DEBUG}>
                     $<$<CONFIG:Debug>:${libcap_libcap_cap_DEPENDENCIES_DEBUG}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'libcap_libcap_cap_DEPS_TARGET' to all of them
        conan_package_library_targets("${libcap_libcap_cap_LIBS_DEBUG}"
                              "${libcap_libcap_cap_LIB_DIRS_DEBUG}"
                              "${libcap_libcap_cap_BIN_DIRS_DEBUG}" # package_bindir
                              "${libcap_libcap_cap_LIBRARY_TYPE_DEBUG}"
                              "${libcap_libcap_cap_IS_HOST_WINDOWS_DEBUG}"
                              libcap_libcap_cap_DEPS_TARGET
                              libcap_libcap_cap_LIBRARIES_TARGETS
                              "_DEBUG"
                              "libcap_libcap_cap"
                              "${libcap_libcap_cap_NO_SONAME_MODE_DEBUG}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET libcap::cap
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${libcap_libcap_cap_OBJECTS_DEBUG}>
                     $<$<CONFIG:Debug>:${libcap_libcap_cap_LIBRARIES_TARGETS}>
                     )

        if("${libcap_libcap_cap_LIBS_DEBUG}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET libcap::cap
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         libcap_libcap_cap_DEPS_TARGET)
        endif()

        set_property(TARGET libcap::cap APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Debug>:${libcap_libcap_cap_LINKER_FLAGS_DEBUG}>)
        set_property(TARGET libcap::cap APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Debug>:${libcap_libcap_cap_INCLUDE_DIRS_DEBUG}>)
        set_property(TARGET libcap::cap APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Debug>:${libcap_libcap_cap_LIB_DIRS_DEBUG}>)
        set_property(TARGET libcap::cap APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Debug>:${libcap_libcap_cap_COMPILE_DEFINITIONS_DEBUG}>)
        set_property(TARGET libcap::cap APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Debug>:${libcap_libcap_cap_COMPILE_OPTIONS_DEBUG}>)

    ########## AGGREGATED GLOBAL TARGET WITH THE COMPONENTS #####################
    set_property(TARGET libcap::libcap APPEND PROPERTY INTERFACE_LINK_LIBRARIES libcap::cap)

########## For the modules (FindXXX)
set(libcap_LIBRARIES_DEBUG libcap::libcap)
