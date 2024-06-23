# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(pulseaudio_FRAMEWORKS_FOUND_DEBUG "") # Will be filled later
conan_find_apple_frameworks(pulseaudio_FRAMEWORKS_FOUND_DEBUG "${pulseaudio_FRAMEWORKS_DEBUG}" "${pulseaudio_FRAMEWORK_DIRS_DEBUG}")

set(pulseaudio_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET pulseaudio_DEPS_TARGET)
    add_library(pulseaudio_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET pulseaudio_DEPS_TARGET
             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Debug>:${pulseaudio_FRAMEWORKS_FOUND_DEBUG}>
             $<$<CONFIG:Debug>:${pulseaudio_SYSTEM_LIBS_DEBUG}>
             $<$<CONFIG:Debug>:Iconv::Iconv;SndFile::sndfile;libcap::libcap;libtool::libtool;ALSA::ALSA;xorg::xorg;openssl::openssl;pulseaudio::pulse>)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### pulseaudio_DEPS_TARGET to all of them
conan_package_library_targets("${pulseaudio_LIBS_DEBUG}"    # libraries
                              "${pulseaudio_LIB_DIRS_DEBUG}" # package_libdir
                              "${pulseaudio_BIN_DIRS_DEBUG}" # package_bindir
                              "${pulseaudio_LIBRARY_TYPE_DEBUG}"
                              "${pulseaudio_IS_HOST_WINDOWS_DEBUG}"
                              pulseaudio_DEPS_TARGET
                              pulseaudio_LIBRARIES_TARGETS  # out_libraries_targets
                              "_DEBUG"
                              "pulseaudio"    # package_name
                              "${pulseaudio_NO_SONAME_MODE_DEBUG}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${pulseaudio_BUILD_DIRS_DEBUG} ${CMAKE_MODULE_PATH})

########## COMPONENTS TARGET PROPERTIES Debug ########################################

    ########## COMPONENT pulseaudio::pulse-simple #############

        set(pulseaudio_pulseaudio_pulse-simple_FRAMEWORKS_FOUND_DEBUG "")
        conan_find_apple_frameworks(pulseaudio_pulseaudio_pulse-simple_FRAMEWORKS_FOUND_DEBUG "${pulseaudio_pulseaudio_pulse-simple_FRAMEWORKS_DEBUG}" "${pulseaudio_pulseaudio_pulse-simple_FRAMEWORK_DIRS_DEBUG}")

        set(pulseaudio_pulseaudio_pulse-simple_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET pulseaudio_pulseaudio_pulse-simple_DEPS_TARGET)
            add_library(pulseaudio_pulseaudio_pulse-simple_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET pulseaudio_pulseaudio_pulse-simple_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${pulseaudio_pulseaudio_pulse-simple_FRAMEWORKS_FOUND_DEBUG}>
                     $<$<CONFIG:Debug>:${pulseaudio_pulseaudio_pulse-simple_SYSTEM_LIBS_DEBUG}>
                     $<$<CONFIG:Debug>:${pulseaudio_pulseaudio_pulse-simple_DEPENDENCIES_DEBUG}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'pulseaudio_pulseaudio_pulse-simple_DEPS_TARGET' to all of them
        conan_package_library_targets("${pulseaudio_pulseaudio_pulse-simple_LIBS_DEBUG}"
                              "${pulseaudio_pulseaudio_pulse-simple_LIB_DIRS_DEBUG}"
                              "${pulseaudio_pulseaudio_pulse-simple_BIN_DIRS_DEBUG}" # package_bindir
                              "${pulseaudio_pulseaudio_pulse-simple_LIBRARY_TYPE_DEBUG}"
                              "${pulseaudio_pulseaudio_pulse-simple_IS_HOST_WINDOWS_DEBUG}"
                              pulseaudio_pulseaudio_pulse-simple_DEPS_TARGET
                              pulseaudio_pulseaudio_pulse-simple_LIBRARIES_TARGETS
                              "_DEBUG"
                              "pulseaudio_pulseaudio_pulse-simple"
                              "${pulseaudio_pulseaudio_pulse-simple_NO_SONAME_MODE_DEBUG}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET pulseaudio::pulse-simple
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${pulseaudio_pulseaudio_pulse-simple_OBJECTS_DEBUG}>
                     $<$<CONFIG:Debug>:${pulseaudio_pulseaudio_pulse-simple_LIBRARIES_TARGETS}>
                     )

        if("${pulseaudio_pulseaudio_pulse-simple_LIBS_DEBUG}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET pulseaudio::pulse-simple
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         pulseaudio_pulseaudio_pulse-simple_DEPS_TARGET)
        endif()

        set_property(TARGET pulseaudio::pulse-simple APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Debug>:${pulseaudio_pulseaudio_pulse-simple_LINKER_FLAGS_DEBUG}>)
        set_property(TARGET pulseaudio::pulse-simple APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Debug>:${pulseaudio_pulseaudio_pulse-simple_INCLUDE_DIRS_DEBUG}>)
        set_property(TARGET pulseaudio::pulse-simple APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Debug>:${pulseaudio_pulseaudio_pulse-simple_LIB_DIRS_DEBUG}>)
        set_property(TARGET pulseaudio::pulse-simple APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Debug>:${pulseaudio_pulseaudio_pulse-simple_COMPILE_DEFINITIONS_DEBUG}>)
        set_property(TARGET pulseaudio::pulse-simple APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Debug>:${pulseaudio_pulseaudio_pulse-simple_COMPILE_OPTIONS_DEBUG}>)

    ########## COMPONENT pulseaudio::pulse #############

        set(pulseaudio_pulseaudio_pulse_FRAMEWORKS_FOUND_DEBUG "")
        conan_find_apple_frameworks(pulseaudio_pulseaudio_pulse_FRAMEWORKS_FOUND_DEBUG "${pulseaudio_pulseaudio_pulse_FRAMEWORKS_DEBUG}" "${pulseaudio_pulseaudio_pulse_FRAMEWORK_DIRS_DEBUG}")

        set(pulseaudio_pulseaudio_pulse_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET pulseaudio_pulseaudio_pulse_DEPS_TARGET)
            add_library(pulseaudio_pulseaudio_pulse_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET pulseaudio_pulseaudio_pulse_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${pulseaudio_pulseaudio_pulse_FRAMEWORKS_FOUND_DEBUG}>
                     $<$<CONFIG:Debug>:${pulseaudio_pulseaudio_pulse_SYSTEM_LIBS_DEBUG}>
                     $<$<CONFIG:Debug>:${pulseaudio_pulseaudio_pulse_DEPENDENCIES_DEBUG}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'pulseaudio_pulseaudio_pulse_DEPS_TARGET' to all of them
        conan_package_library_targets("${pulseaudio_pulseaudio_pulse_LIBS_DEBUG}"
                              "${pulseaudio_pulseaudio_pulse_LIB_DIRS_DEBUG}"
                              "${pulseaudio_pulseaudio_pulse_BIN_DIRS_DEBUG}" # package_bindir
                              "${pulseaudio_pulseaudio_pulse_LIBRARY_TYPE_DEBUG}"
                              "${pulseaudio_pulseaudio_pulse_IS_HOST_WINDOWS_DEBUG}"
                              pulseaudio_pulseaudio_pulse_DEPS_TARGET
                              pulseaudio_pulseaudio_pulse_LIBRARIES_TARGETS
                              "_DEBUG"
                              "pulseaudio_pulseaudio_pulse"
                              "${pulseaudio_pulseaudio_pulse_NO_SONAME_MODE_DEBUG}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET pulseaudio::pulse
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${pulseaudio_pulseaudio_pulse_OBJECTS_DEBUG}>
                     $<$<CONFIG:Debug>:${pulseaudio_pulseaudio_pulse_LIBRARIES_TARGETS}>
                     )

        if("${pulseaudio_pulseaudio_pulse_LIBS_DEBUG}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET pulseaudio::pulse
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         pulseaudio_pulseaudio_pulse_DEPS_TARGET)
        endif()

        set_property(TARGET pulseaudio::pulse APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Debug>:${pulseaudio_pulseaudio_pulse_LINKER_FLAGS_DEBUG}>)
        set_property(TARGET pulseaudio::pulse APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Debug>:${pulseaudio_pulseaudio_pulse_INCLUDE_DIRS_DEBUG}>)
        set_property(TARGET pulseaudio::pulse APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Debug>:${pulseaudio_pulseaudio_pulse_LIB_DIRS_DEBUG}>)
        set_property(TARGET pulseaudio::pulse APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Debug>:${pulseaudio_pulseaudio_pulse_COMPILE_DEFINITIONS_DEBUG}>)
        set_property(TARGET pulseaudio::pulse APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Debug>:${pulseaudio_pulseaudio_pulse_COMPILE_OPTIONS_DEBUG}>)

    ########## AGGREGATED GLOBAL TARGET WITH THE COMPONENTS #####################
    set_property(TARGET pulseaudio::pulseaudio APPEND PROPERTY INTERFACE_LINK_LIBRARIES pulseaudio::pulse-simple)
    set_property(TARGET pulseaudio::pulseaudio APPEND PROPERTY INTERFACE_LINK_LIBRARIES pulseaudio::pulse)

########## For the modules (FindXXX)
set(pulseaudio_LIBRARIES_DEBUG pulseaudio::pulseaudio)
