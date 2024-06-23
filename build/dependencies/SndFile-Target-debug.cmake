# Avoid multiple calls to find_package to append duplicated properties to the targets
include_guard()########### VARIABLES #######################################################################
#############################################################################################
set(libsndfile_FRAMEWORKS_FOUND_DEBUG "") # Will be filled later
conan_find_apple_frameworks(libsndfile_FRAMEWORKS_FOUND_DEBUG "${libsndfile_FRAMEWORKS_DEBUG}" "${libsndfile_FRAMEWORK_DIRS_DEBUG}")

set(libsndfile_LIBRARIES_TARGETS "") # Will be filled later


######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
if(NOT TARGET libsndfile_DEPS_TARGET)
    add_library(libsndfile_DEPS_TARGET INTERFACE IMPORTED)
endif()

set_property(TARGET libsndfile_DEPS_TARGET
             APPEND PROPERTY INTERFACE_LINK_LIBRARIES
             $<$<CONFIG:Debug>:${libsndfile_FRAMEWORKS_FOUND_DEBUG}>
             $<$<CONFIG:Debug>:${libsndfile_SYSTEM_LIBS_DEBUG}>
             $<$<CONFIG:Debug>:Ogg::ogg;Vorbis::vorbis;Vorbis::vorbisenc;flac::flac;Opus::opus;mpg123::mpg123;libmp3lame::libmp3lame>)

####### Find the libraries declared in cpp_info.libs, create an IMPORTED target for each one and link the
####### libsndfile_DEPS_TARGET to all of them
conan_package_library_targets("${libsndfile_LIBS_DEBUG}"    # libraries
                              "${libsndfile_LIB_DIRS_DEBUG}" # package_libdir
                              "${libsndfile_BIN_DIRS_DEBUG}" # package_bindir
                              "${libsndfile_LIBRARY_TYPE_DEBUG}"
                              "${libsndfile_IS_HOST_WINDOWS_DEBUG}"
                              libsndfile_DEPS_TARGET
                              libsndfile_LIBRARIES_TARGETS  # out_libraries_targets
                              "_DEBUG"
                              "libsndfile"    # package_name
                              "${libsndfile_NO_SONAME_MODE_DEBUG}")  # soname

# FIXME: What is the result of this for multi-config? All configs adding themselves to path?
set(CMAKE_MODULE_PATH ${libsndfile_BUILD_DIRS_DEBUG} ${CMAKE_MODULE_PATH})

########## COMPONENTS TARGET PROPERTIES Debug ########################################

    ########## COMPONENT SndFile::sndfile #############

        set(libsndfile_SndFile_sndfile_FRAMEWORKS_FOUND_DEBUG "")
        conan_find_apple_frameworks(libsndfile_SndFile_sndfile_FRAMEWORKS_FOUND_DEBUG "${libsndfile_SndFile_sndfile_FRAMEWORKS_DEBUG}" "${libsndfile_SndFile_sndfile_FRAMEWORK_DIRS_DEBUG}")

        set(libsndfile_SndFile_sndfile_LIBRARIES_TARGETS "")

        ######## Create an interface target to contain all the dependencies (frameworks, system and conan deps)
        if(NOT TARGET libsndfile_SndFile_sndfile_DEPS_TARGET)
            add_library(libsndfile_SndFile_sndfile_DEPS_TARGET INTERFACE IMPORTED)
        endif()

        set_property(TARGET libsndfile_SndFile_sndfile_DEPS_TARGET
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${libsndfile_SndFile_sndfile_FRAMEWORKS_FOUND_DEBUG}>
                     $<$<CONFIG:Debug>:${libsndfile_SndFile_sndfile_SYSTEM_LIBS_DEBUG}>
                     $<$<CONFIG:Debug>:${libsndfile_SndFile_sndfile_DEPENDENCIES_DEBUG}>
                     )

        ####### Find the libraries declared in cpp_info.component["xxx"].libs,
        ####### create an IMPORTED target for each one and link the 'libsndfile_SndFile_sndfile_DEPS_TARGET' to all of them
        conan_package_library_targets("${libsndfile_SndFile_sndfile_LIBS_DEBUG}"
                              "${libsndfile_SndFile_sndfile_LIB_DIRS_DEBUG}"
                              "${libsndfile_SndFile_sndfile_BIN_DIRS_DEBUG}" # package_bindir
                              "${libsndfile_SndFile_sndfile_LIBRARY_TYPE_DEBUG}"
                              "${libsndfile_SndFile_sndfile_IS_HOST_WINDOWS_DEBUG}"
                              libsndfile_SndFile_sndfile_DEPS_TARGET
                              libsndfile_SndFile_sndfile_LIBRARIES_TARGETS
                              "_DEBUG"
                              "libsndfile_SndFile_sndfile"
                              "${libsndfile_SndFile_sndfile_NO_SONAME_MODE_DEBUG}")


        ########## TARGET PROPERTIES #####################################
        set_property(TARGET SndFile::sndfile
                     APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                     $<$<CONFIG:Debug>:${libsndfile_SndFile_sndfile_OBJECTS_DEBUG}>
                     $<$<CONFIG:Debug>:${libsndfile_SndFile_sndfile_LIBRARIES_TARGETS}>
                     )

        if("${libsndfile_SndFile_sndfile_LIBS_DEBUG}" STREQUAL "")
            # If the component is not declaring any "cpp_info.components['foo'].libs" the system, frameworks etc are not
            # linked to the imported targets and we need to do it to the global target
            set_property(TARGET SndFile::sndfile
                         APPEND PROPERTY INTERFACE_LINK_LIBRARIES
                         libsndfile_SndFile_sndfile_DEPS_TARGET)
        endif()

        set_property(TARGET SndFile::sndfile APPEND PROPERTY INTERFACE_LINK_OPTIONS
                     $<$<CONFIG:Debug>:${libsndfile_SndFile_sndfile_LINKER_FLAGS_DEBUG}>)
        set_property(TARGET SndFile::sndfile APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
                     $<$<CONFIG:Debug>:${libsndfile_SndFile_sndfile_INCLUDE_DIRS_DEBUG}>)
        set_property(TARGET SndFile::sndfile APPEND PROPERTY INTERFACE_LINK_DIRECTORIES
                     $<$<CONFIG:Debug>:${libsndfile_SndFile_sndfile_LIB_DIRS_DEBUG}>)
        set_property(TARGET SndFile::sndfile APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS
                     $<$<CONFIG:Debug>:${libsndfile_SndFile_sndfile_COMPILE_DEFINITIONS_DEBUG}>)
        set_property(TARGET SndFile::sndfile APPEND PROPERTY INTERFACE_COMPILE_OPTIONS
                     $<$<CONFIG:Debug>:${libsndfile_SndFile_sndfile_COMPILE_OPTIONS_DEBUG}>)

    ########## AGGREGATED GLOBAL TARGET WITH THE COMPONENTS #####################
    set_property(TARGET SndFile::sndfile APPEND PROPERTY INTERFACE_LINK_LIBRARIES SndFile::sndfile)

########## For the modules (FindXXX)
set(libsndfile_LIBRARIES_DEBUG SndFile::sndfile)
