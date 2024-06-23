# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(pulseaudio_FRAMEWORKS_FOUND_RELEASE "") # Will be filled later
conan_find_apple_frameworks(pulseaudio_FRAMEWORKS_FOUND_RELEASE "${pulseaudio_FRAMEWORKS_RELEASE}" "${pulseaudio_FRAMEWORK_DIRS_RELEASE}")

set(pulseaudio_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET pulseaudio_DEPS_TARGET)
    add_library(pulseaudio_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET pulseaudio_DEPS_TARGET
             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Release>:${pulseaudio_FRAMEWORKS_FOUND_RELEASE}>
             $<$<CONFIG:Release>:${pulseaudio_SYSTEM_LIBS_RELEASE}>
             $<$<CONFIG:Release>:Iconv::Iconv;SndFile::sndfile;libcap::libcap;libtool::libtool;ALSA::ALSA;xorg::xorg;openssl::openssl;pulseaudio::pulse>)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### pulseaudio_DEPS_TARGET to all of them
conan_package_library_targets("${pulseaudio_LIBS_RELEASE}"    # libraries
                              "${pulseaudio_LIB_DIRS_RELEASE}" # package_libdir
                              "${pulseaudio_BIN_DIRS_RELEASE}" # package_bindir
                              "${pulseaudio_LIBRARY_TYPE_RELEASE}"
                              "${pulseaudio_IS_HOST_WINDOWS_RELEASE}"
                              pulseaudio_DEPS_TARGET
                              pulseaudio_LIBRARIES_TARGETS  # out_libraries_targets
                              "_RELEASE"
                              "pulseaudio"    # package_name
                              "${pulseaudio_NO_SONAME_MODE_RELEASE}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${pulseaudio_BUILD_DIRS_RELEASE} ${CMAKE_MODULE_PATH})

########## COMPONENTS TARGET PROPERTIES Release ########################################

    ########## COMPONENT pulseaudio::pulse-simple #############

        set(pulseaudio_pulseaudio_pulse-simple_FRAMEWORKS_FOUND_RELEASE "")
        conan_find_apple_frameworks(pulseaudio_pulseaudio_pulse-simple_FRAMEWORKS_FOUND_RELEASE "${pulseaudio_pulseaudio_pulse-simple_FRAMEWORKS_RELEASE}" "${pulseaudio_pulseaudio_pulse-simple_FRAMEWORK_DIRS_RELEASE}")

        set(pulseaudio_pulseaudio_pulse-simple_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET pulseaudio_pulseaudio_pulse-simple_DEPS_TARGET)
            add_library(pulseaudio_pulseaudio_pulse-simple_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET pulseaudio_pulseaudio_pulse-simple_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${pulseaudio_pulseaudio_pulse-simple_FRAMEWORKS_FOUND_RELEASE}>
                     $<$<CONFIG:Release>:${pulseaudio_pulseaudio_pulse-simple_SYSTEM_LIBS_RELEASE}>
                     $<$<CONFIG:Release>:${pulseaudio_pulseaudio_pulse-simple_DEPENDENCIES_RELEASE}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'pulseaudio_pulseaudio_pulse-simple_DEPS_TARGET' to all of them
        conan_package_library_targets("${pulseaudio_pulseaudio_pulse-simple_LIBS_RELEASE}"
                              "${pulseaudio_pulseaudio_pulse-simple_LIB_DIRS_RELEASE}"
                              "${pulseaudio_pulseaudio_pulse-simple_BIN_DIRS_RELEASE}" # package_bindir
                              "${pulseaudio_pulseaudio_pulse-simple_LIBRARY_TYPE_RELEASE}"
                              "${pulseaudio_pulseaudio_pulse-simple_IS_HOST_WINDOWS_RELEASE}"
                              pulseaudio_pulseaudio_pulse-simple_DEPS_TARGET
                              pulseaudio_pulseaudio_pulse-simple_LIBRARIES_TARGETS
                              "_RELEASE"
                              "pulseaudio_pulseaudio_pulse-simple"
                              "${pulseaudio_pulseaudio_pulse-simple_NO_SONAME_MODE_RELEASE}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET pulseaudio::pulse-simple
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${pulseaudio_pulseaudio_pulse-simple_OBJECTS_RELEASE}>
                     $<$<CONFIG:Release>:${pulseaudio_pulseaudio_pulse-simple_LIBRARIES_TARGETS}>
                     )

        if("${pulseaudio_pulseaudio_pulse-simple_LIBS_RELEASE}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET pulseaudio::pulse-simple
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         pulseaudio_pulseaudio_pulse-simple_DEPS_TARGET)
        endif()

        set_property(TARGET pulseaudio::pulse-simple APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Release>:${pulseaudio_pulseaudio_pulse-simple_LINKER_FLAGS_RELEASE}>)
        set_property(TARGET pulseaudio::pulse-simple APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Release>:${pulseaudio_pulseaudio_pulse-simple_INCLUDE_DIRS_RELEASE}>)
        set_property(TARGET pulseaudio::pulse-simple APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Release>:${pulseaudio_pulseaudio_pulse-simple_LIB_DIRS_RELEASE}>)
        set_property(TARGET pulseaudio::pulse-simple APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Release>:${pulseaudio_pulseaudio_pulse-simple_COMPILE_DEFINITIONS_RELEASE}>)
        set_property(TARGET pulseaudio::pulse-simple APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Release>:${pulseaudio_pulseaudio_pulse-simple_COMPILE_OPTIONS_RELEASE}>)

    ########## COMPONENT pulseaudio::pulse #############

        set(pulseaudio_pulseaudio_pulse_FRAMEWORKS_FOUND_RELEASE "")
        conan_find_apple_frameworks(pulseaudio_pulseaudio_pulse_FRAMEWORKS_FOUND_RELEASE "${pulseaudio_pulseaudio_pulse_FRAMEWORKS_RELEASE}" "${pulseaudio_pulseaudio_pulse_FRAMEWORK_DIRS_RELEASE}")

        set(pulseaudio_pulseaudio_pulse_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET pulseaudio_pulseaudio_pulse_DEPS_TARGET)
            add_library(pulseaudio_pulseaudio_pulse_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET pulseaudio_pulseaudio_pulse_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${pulseaudio_pulseaudio_pulse_FRAMEWORKS_FOUND_RELEASE}>
                     $<$<CONFIG:Release>:${pulseaudio_pulseaudio_pulse_SYSTEM_LIBS_RELEASE}>
                     $<$<CONFIG:Release>:${pulseaudio_pulseaudio_pulse_DEPENDENCIES_RELEASE}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'pulseaudio_pulseaudio_pulse_DEPS_TARGET' to all of them
        conan_package_library_targets("${pulseaudio_pulseaudio_pulse_LIBS_RELEASE}"
                              "${pulseaudio_pulseaudio_pulse_LIB_DIRS_RELEASE}"
                              "${pulseaudio_pulseaudio_pulse_BIN_DIRS_RELEASE}" # package_bindir
                              "${pulseaudio_pulseaudio_pulse_LIBRARY_TYPE_RELEASE}"
                              "${pulseaudio_pulseaudio_pulse_IS_HOST_WINDOWS_RELEASE}"
                              pulseaudio_pulseaudio_pulse_DEPS_TARGET
                              pulseaudio_pulseaudio_pulse_LIBRARIES_TARGETS
                              "_RELEASE"
                              "pulseaudio_pulseaudio_pulse"
                              "${pulseaudio_pulseaudio_pulse_NO_SONAME_MODE_RELEASE}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET pulseaudio::pulse
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Release>:${pulseaudio_pulseaudio_pulse_OBJECTS_RELEASE}>
                     $<$<CONFIG:Release>:${pulseaudio_pulseaudio_pulse_LIBRARIES_TARGETS}>
                     )

        if("${pulseaudio_pulseaudio_pulse_LIBS_RELEASE}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET pulseaudio::pulse
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         pulseaudio_pulseaudio_pulse_DEPS_TARGET)
        endif()

        set_property(TARGET pulseaudio::pulse APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Release>:${pulseaudio_pulseaudio_pulse_LINKER_FLAGS_RELEASE}>)
        set_property(TARGET pulseaudio::pulse APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Release>:${pulseaudio_pulseaudio_pulse_INCLUDE_DIRS_RELEASE}>)
        set_property(TARGET pulseaudio::pulse APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Release>:${pulseaudio_pulseaudio_pulse_LIB_DIRS_RELEASE}>)
        set_property(TARGET pulseaudio::pulse APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Release>:${pulseaudio_pulseaudio_pulse_COMPILE_DEFINITIONS_RELEASE}>)
        set_property(TARGET pulseaudio::pulse APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Release>:${pulseaudio_pulseaudio_pulse_COMPILE_OPTIONS_RELEASE}>)

    ########## AGGREGATED GLOBAL TARGET WITH THE COMPONENTS #####################
    set_property(TARGET pulseaudio::pulseaudio APPEND PROPERTY INTERFACE_LINK_LIBRARIES pulseaudio::pulse-simple)
    set_property(TARGET pulseaudio::pulseaudio APPEND PROPERTY INTERFACE_LINK_LIBRARIES pulseaudio::pulse)

########## For the modules (FindXXX)
set(pulseaudio_LIBRARIES_RELEASE pulseaudio::pulseaudio)
