########## MACROS ###########################################################################
#############################################################################################

# Requires CMake > 3.15
if(${CMAKE_VERSION} VERSION_LESS "3.15")
    message(FATAL_ERROR "The 'CMakeDeps' generator only works with CMake >= 3.15")
endif()

if(pulseaudio_FIND_QUIETLY)
    set(pulseaudio_MESSAGE_MODE VERBOSE)
else()
    set(pulseaudio_MESSAGE_MODE STATUS)
endif()

include(${CMAKE_CURRENT_LIST_DIR}/cmakedeps_macros.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/pulseaudioTargets.cmake)
include(CMakeFindDependencyMacro)

check_build_type_defined()

foreach(_DEPENDENCY ${pulseaudio_FIND_DEPENDENCY_NAMES} )
    # Check that we have not already called a find_package with the transitive dependency
    if(NOT ${_DEPENDENCY}_FOUND)
        find_dependency(${_DEPENDENCY} REQUIRED ${${_DEPENDENCY}_FIND_MODE})
    endif()
endforeach()

set(pulseaudio_VERSION_STRING "14.2")
set(pulseaudio_INCLUDE_DIRS ${pulseaudio_INCLUDE_DIRS_DEBUG} )
set(pulseaudio_INCLUDE_DIR ${pulseaudio_INCLUDE_DIRS_DEBUG} )
set(pulseaudio_LIBRARIES ${pulseaudio_LIBRARIES_DEBUG} )
set(pulseaudio_DEFINITIONS ${pulseaudio_DEFINITIONS_DEBUG} )


# Only the last installed configuration BUILD_MODULES are included to avoid the collision
foreach(_BUILD_MODULE ${pulseaudio_BUILD_MODULES_PATHS_DEBUG} )
    message(${pulseaudio_MESSAGE_MODE} "Conan: Including build module from '${_BUILD_MODULE}'")
    include(${_BUILD_MODULE})
endforeach()


